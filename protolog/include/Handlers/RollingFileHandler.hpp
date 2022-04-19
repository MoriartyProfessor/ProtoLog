#ifndef ROLLIING_FILE_HANDLER_HPP
#define ROLLIING_FILE_HANDLER_HPP

#include <stdio.h>
#include <filesystem>
#include <algorithm>
#include <FileHandler.hpp>

namespace Protolog
{
    class RollingFileHandler : public FileHandler
    {
        uint max_size;
        uint max_files;
        std::string arch_base;
        inline static std::filesystem::path arch_dir{"./archive/"};

        void init_arch_base()
        {
            std::size_t dot_pos = filename.find(".");
            if(dot_pos!=std::string::npos)
                arch_base = filename.substr(0, dot_pos);
            else
                arch_base = filename;
        }
        std::vector<std::string> get_arch_names()
        {
            std::vector<std::string> arch_names;
            for(const auto& e : std::filesystem::directory_iterator(arch_dir)) 
            {
                std::string curr_fname = e.path().filename();
                if(arch_base == curr_fname.substr(0, arch_base.size()))
                {
                    arch_names.push_back(curr_fname);
                }
            }
            std::sort(arch_names.begin(), arch_names.end(), [](const std::string& str1, const std::string& str2)
            {
                if(str1.size() != str2.size())
                    return str1.size() < str2.size();
                return str1 < str2;
            });
            return arch_names;
        }
        std::string next_arch_name(const std::vector<std::string>& arch_names)
        {
            if(arch_names.size() == 0)
                return arch_base + std::to_string(1);
            std::string suffix_str = arch_names.back().substr(arch_base.size(), arch_names.back().size() - arch_base.size());
            uint suffix_i = std::stoi(suffix_str);
            return arch_base + std::to_string(suffix_i+1);
        }

        void move_to_archive(const std::string& arch_name)
        {
            std::ofstream n_archlog{path_cat(arch_dir, arch_name)};
            std::ifstream fin{filename};
            std::string line;
            while(std::getline(fin, line))
            {
                n_archlog << line << "\n";
            }
            std::filesystem::resize_file(filename, 0);
        }

        void rotate()
        {
            std::vector<std::string> arch_names = get_arch_names();
            if(arch_names.size() != max_files)
            {
                std::string next = next_arch_name(arch_names);
                arch_names.push_back(next);
            }
            if(arch_names.size()>=2)
            {
                for(auto it = arch_names.end()-2; it >= arch_names.begin(); --it)
                {
                    rename(path_cat(arch_dir, *it).data(), path_cat(arch_dir, *std::next(it)).data());
                }
            }
            move_to_archive(arch_names.front());
        }

        void archive()
        {
            std::filesystem::path curr_path{"./"};
            if(!dir_exists(curr_path, "archive"))
            {
                std::filesystem::create_directory(arch_dir);
            }
            rotate();
        }

        public:
        RollingFileHandler(const std::string& filename, uint max_size = 0, uint max_file = 16)
        :FileHandler{filename, std::ios::app}, max_size{max_size},  max_files{max_file}
        {
            if(max_file < 4)
                max_file = 4;
            init_arch_base();
        }

        virtual void write(const LogRecord& record) override
        {
            std::string formatted_record = formatter->format_record(record);
            if(max_size != 0 && file_size(fout) > max_size)
            {
                archive();
            }
            fout<<formatter->format_record(record)<<std::endl;
        }
    };
}

#endif
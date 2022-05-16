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
        std::filesystem::path arch_dir;

        std::vector<std::string> get_arch_names()
        {
            std::vector<std::string> arch_names;
            for(const auto& file_it : std::filesystem::directory_iterator(arch_dir)) 
            {
                std::string curr_fname = file_it.path().filename();
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
            fout.close();
            rename(filename.c_str(), path_cat(arch_dir, arch_name).c_str());
            fout.open(filename, std::ios::out);
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
            if(!std::filesystem::exists(arch_dir))
                std::filesystem::create_directory(arch_dir);
            rotate();
        }

        public:
        RollingFileHandler(const std::string& filename, const std::string& arch_path = "./archive/", uint max_size = 0, uint max_file = 128)
        :FileHandler{filename, std::ios::app}, arch_dir{arch_path}, max_size{max_size},  max_files{max_file}
        {
            if(max_file < 4)
                max_file = 4;
            arch_base = filename;
        }

        virtual void write(const LogRecord& record) override
        {
            if(max_size != 0 && file_size(fout) > max_size)
            {
                archive();
            }
            fout<<formatter->format_record(record)<<std::endl;
        }
    };
}

#endif
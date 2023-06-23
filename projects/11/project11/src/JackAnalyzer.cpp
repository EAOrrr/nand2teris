#include"tools.h"
#include"compileEngine.h"
#include"JackTokenizer.h"
#include<dirent.h>
std::vector<std::string> GetListFiles(const std::string& path, const std::string& exten = "*")
{
    std::vector<std::string> list;
    list.clear();
 
    DIR* dp = nullptr;
    struct dirent* dirp = nullptr;
    if ((dp = opendir(path.c_str())) == nullptr) {
        return list;
    }
 
    while ((dirp = readdir(dp)) != nullptr) {
        if (dirp->d_type == DT_REG) {
            if (exten.compare("*") == 0)
                list.emplace_back(static_cast<std::string>(dirp->d_name));
            else
                if (std::string(dirp->d_name).find(exten) != std::string::npos)
                    list.emplace_back(static_cast<std::string>(dirp->d_name));
        }
    }
 
    closedir(dp);
 
    return list;
}

int main(int argc, char** argv){
    
    if(argc != 2){
        std::cerr << "invaild number of arguments" << std::endl;
        return EXIT_FAILURE;
    }
    std::string inname = argv[1];
    if(inname.find(".jack") != std::string::npos){
        CompileEngine analyzer(inname);
        analyzer.compileClass();
    }
    else{
        // the filname is a dir
        std::vector<std::string> files = GetListFiles(argv[1]);
        // std::cout << "here" << std::endl;
        std::string path = argv[1];
        for(auto& file: files){
            std::string filename = path + "/" + file;
            size_t pos = filename.find(".jack");
            if(pos == std::string::npos) continue;  // it is not a vm file
            else{
                CompileEngine analyzer(filename);
                analyzer.compileClass();
            }
        }
    }
    return EXIT_SUCCESS;
}
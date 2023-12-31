#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <d3dcompiler.h>

// To colorize text.
#include "termcolor.hpp"

#define OUTPUTS  1
#define DEFINES  2
#define NONE    -1

int main(int argc, char** argv);
std::vector<std::string> split(std::string, char);

int main(int argc, char** argv) {
    using namespace termcolor;
    
    if(argc == 1) {
        std::cout << yellow << "[HLSLC]" << red << "[ERROR]: No arguments!" << reset << std::endl;
        return -1;
    }
    
    std::string input, target, entryPoint = "main", str_temp = "";
    std::vector<std::string> outputFiles, defines, vec_temp;
    
    BOOL debug = FALSE;
    INT8 currentSwitch = NONE;
    
    for(register UINT32 i = 1; i < argc; i++) {
        str_temp = argv[i];
        
        if(str_temp.find("-O") != std::string::npos) 
            currentSwitch = OUTPUTS;
        else if(str_temp.find("-I") != std::string::npos) 
            currentSwitch = NONE;
        else if(str_temp.find("-DEBUG") != std::string::npos) 
            debug = true;
        else if(str_temp.find("-D") != std::string::npos) 
            currentSwitch = DEFINES;
        else if(str_temp.find("-T") != std::string::npos) {
            if(target.size() == 0u)
                target = str_temp.replace(0, 2, "");
            else {
                std::cout << yellow << "[HLSLC]" << red << "[ERROR]: Multiple targets defined! Last target: \"" << blue << target << red << "\"" << reset << std::endl;
                return -1;
            }
        }
        else if(str_temp.find("-E") != std::string::npos) {
            if(entryPoint == "main")
                entryPoint = str_temp.replace(0, 2, "");
            else {
                std::cout << yellow << "[HLSLC]" << red << "[ERROR]: Multiple entry points defined! Last EP: \"" << blue << entryPoint << red << "\"" << reset << std::endl;
                return -1;
            }
        }
        else {
            if(currentSwitch == NONE) {
                if(input.size() == 0u) 
                    input = str_temp;
                else {
                    std::cout << yellow << "[HLSLC]" << red << "[ERROR]: Input file is already defined: \"" << blue << input << red << "\"" << reset << std::endl;
                    return -1;
                }
            }
            else if(currentSwitch == OUTPUTS) 
                outputFiles.push_back(str_temp);
            else if(currentSwitch == DEFINES) {
                vec_temp = split(str_temp, ',');

                for(register UINT32 i = 0; i < vec_temp.size(); i++) {
                    std::vector<std::string> splited = split(vec_temp[i], '=');

                    defines.insert(defines.end(), { splited[0], splited[1] });
                }
            }
        }
    }
    
    if(input.size() == 0u) {
        std::cout << yellow << "[HLSLC]" << red << "[ERROR]: No input file!" << reset << std::endl;
        return -1;
    }
    
    if(target.size() == 0u) {
        std::cout << yellow << "[HLSLC]" << red << "[ERROR]: No target defined!" << reset << std::endl;
        return -1;
    }
    
    if(outputFiles.size() == 0u) {
        std::cout << yellow << "[HLSLC]" << red << "[ERROR]: Expected minimal output files!" << reset << std::endl;
        return -1;
    }
    
    std::ifstream ifs(input.c_str());
    
    if(!ifs.good()) {
        std::cout << yellow << "[HLSLC]" << red << "[ERROR]: Input file does not exists!" << reset << std::endl;
        return -1;
    }
    
    D3D_SHADER_MACRO* x_defines = nullptr;

    UINT32 definesSize = defines.size();
    
    if(definesSize != 0u) {
        UINT32 i = 0, size = definesSize / 2;
        x_defines = new D3D_SHADER_MACRO[size + 1];
    
        for(; i < size; i++) 
            x_defines[i] = { defines[2 * i].c_str(), defines[2 * i + 1].c_str() };

        // Termination entry.
        x_defines[i] = { nullptr, nullptr };
    }
    
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
    
    if(debug == TRUE) 
        flags |= D3DCOMPILE_DEBUG;
    
    ID3DBlob* code   = nullptr;
    ID3DBlob* errors = nullptr;
    
    HRESULT hr = D3DCompileFromFile(std::wstring(input.begin(), input.end()).c_str(), x_defines, (ID3DInclude*)(UINT_PTR) 1, entryPoint.c_str(), target.c_str(), flags, 0, &code, &errors);
    
    if(FAILED(hr)) {
        if(errors) 
            std::cout << green << reinterpret_cast<const char*>(errors -> GetBufferPointer()) << reset << std::endl;
        
        std::cout << yellow << "[HLSLC]" << red << "[ERROR]: Failed to compile shader!" << yellow << " Try fixing the errors." << reset << std::endl;
    } else {
        for(std::string str : outputFiles) 
            D3DWriteBlobToFile(code, std::wstring(str.begin(), str.end()).c_str(), TRUE);
    }

    if(definesSize != 0u) 
        delete[] x_defines;

    // Release the blobs.
    
    if(code != nullptr) 
        code -> Release();
    
    if(errors != nullptr) 
        errors -> Release();
    
    return 0;
}

std::vector<std::string> split(std::string str, char token) {
    std::vector<std::string> vec;
    std::string temp;
    std::istringstream iss(str);
    
    while(std::getline(iss, temp, token)) 
        vec.push_back(temp);
    
    return vec;
}

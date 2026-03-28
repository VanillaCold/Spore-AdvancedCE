import os
import re

invalid_folders = r"(^\..*)|(^Release$)|(^Debug$)"
invalid_names = r"(^\.)|(^stdafx\.(cpp|h)$)|(^targetver\.h$)|(^dllmain\.cpp$)"
valid_extensions = r".*\.(h|cpp)$"

def SearchForFiles(path: str = "."):
    outputs = []
    fsobjects = os.listdir(path)
    for newPath in fsobjects:
        if (os.path.isfile(path+"/"+newPath)):
            if (not (re.match(invalid_names, newPath)) and (re.match(valid_extensions, newPath))):
                outputs.append(path + "/" + newPath)
                #print(path + "/" + newPath)
            continue
        else:
            if (not re.match(invalid_folders, newPath)):
                #print(path + "/" + newPath)
                outputs += SearchForFiles(path+"/"+newPath)
            continue
    return outputs

def MakeVcxproj(name: str, headers: list, codefiles: list):
    headerText = ""
    codeText = ""
    # ====INCLUDE_HEADERS
    # ====INCLUDE_COMPILES
    for headerName in headers:
        headerText += '<ClInclude Include="'+headerName+'"/>\n';
    for codeName in codefiles:
        codeText += '<ClCompile Include="'+codeName+'"/>\n'

    fileHandle = open("./.projectTemplate/template.xml", "r")
    text = fileHandle.read()
    fileHandle.close();

    text = text.replace("====INCLUDE_HEADERS\n", headerText)
    text = text.replace("====INCLUDE_COMPILES\n", codeText)
    text = text.replace("====PROJECT_NAME====", name)

    fileHandle = open(name+".vcxproj", "w")
    fileHandle.write(text)
    fileHandle.close()


files = SearchForFiles();
name = os.getcwd().split(os.sep)[-1];

print (files, "\n"+name)

hr = re.compile(r".*\.h$");
cppr = re.compile(r".*\.cpp$");
headers = list(filter(hr.match, files));
codes = list(filter(cppr.match, files));
print(headers, codes)

MakeVcxproj(name, headers, codes)

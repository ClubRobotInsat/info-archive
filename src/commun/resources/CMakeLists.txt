message("-- Creating storage file")

file(WRITE "embedded" "")
get_filename_component(abs_path "embedded" ABSOLUTE)
set(RESOURCE_FILE_LOCATION ${abs_path} CACHE FILEPATH "The resource file contains files included in the executables")

add_library(Resources EmbeddedFiles.cpp)

# Cette fonction prends un fichier normal et le rends #include-able pour s'affranchir du fichier table_2018.json
# Le pré-processeur copie automagiquement le fichier dans un std::string
function(include_file path)
    file(READ ${path} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(APPEND ${RESOURCE_FILE_LOCATION} "{ \"${path}\", ${content} },\n")
endfunction(include_file)


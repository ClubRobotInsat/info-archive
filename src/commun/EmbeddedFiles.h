#ifndef ROOT_EMBEDDEDFILES_H
#define ROOT_EMBEDDEDFILES_H

#include <map>
#include <string>

/**
 * Cette classe permet de lire des fichiers embarqués dans
 * l'executable. Les fichiers sont sous forme de chaines de
 * caractères dans un fichier généré par CMake.
 */
class EmbeddedFiles {
public:
	EmbeddedFiles() = delete;

	/**
	 * Retourne le contenu du fichier texte embarqué dans
	 * l'executable ayant le chemin passé en paramètres.
	 * @param path Le chemin du fichier tel qu'il a été inclu
	 * dans le CMakeLists.
	 * @return Une chaine de caractère qui correspond au contenu
	 * du fichier.
	 */
	static const std::string& readText(const std::string& path);

private:
	static std::map<std::string, std::string> &files();
};

#endif // ROOT_FILEMANAGER_H

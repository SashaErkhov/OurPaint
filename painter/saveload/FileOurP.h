//
// Created by Eugene Bychkov on 28.09.2024.
//

#ifndef OURPAINT_EXPORTTOFILE_H
#define OURPAINT_EXPORTTOFILE_H

#include <fstream>
#include <vector>
#include "objectInFile.h"
#include <sstream>
#include <algorithm>
#include <queue>

class FileOurP {
    std::vector<objectInFile> m_objects;
    std::vector<objectInFile> parseFile(std::istream& file);

public:
    FileOurP();

    explicit FileOurP(const std::vector<objectInFile> &obj);

    FileOurP(const FileOurP &other);

    FileOurP(FileOurP &&other) noexcept;

    FileOurP &operator=(const FileOurP &other);

    FileOurP &operator=(FileOurP &&other) noexcept;

    void addObject(std::pair<ID, primitive*> &obj);

    void saveToOurP(const std::string &fileName) const;

    void loadFromOurP(const std::string &fileName);

    const std::vector<objectInFile>& getObjects() const;

    std::string to_string() const;

    void loadString(const std::string &basicString);
};


#endif //OURPAINT_EXPORTTOFILE_H

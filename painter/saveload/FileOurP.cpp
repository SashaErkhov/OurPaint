//
// Created by Eugene Bychkov on 28.09.2024.
//

#include "FileOurP.h"

FileOurP::FileOurP() {
    m_objects = std::vector<objectInFile>();
}

FileOurP::FileOurP(FileOurP &&other) noexcept {
    m_objects = std::move(other.m_objects);
}

FileOurP::FileOurP(const FileOurP &other) = default;

FileOurP::FileOurP(const std::vector<objectInFile> &obj) : m_objects(obj) {}

FileOurP &FileOurP::operator=(const FileOurP &other) = default;

FileOurP &FileOurP::operator=(FileOurP &&other) noexcept {
    m_objects = std::move(other.m_objects);
    return *this;
}

void FileOurP::addObject(std::pair<ID, primitive*> &obj) {
    m_objects.emplace_back(obj);
}


void FileOurP::saveToOurP(const std::string &fileName) const {
    std::ofstream file(fileName + ".ourp");
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    file << "Elements: {\n";
    for (const auto &obj: m_objects) {
        file << obj.to_string() << "\n";
    }
    file << "}\n";
    //TODO::add for req
    file.close();
}

const std::vector<objectInFile> &FileOurP::getObjects() const {
        return m_objects;
}
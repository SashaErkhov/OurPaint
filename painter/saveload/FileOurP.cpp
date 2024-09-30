//
// Created by Eugene Bychkov on 28.09.2024.
//

#include "FileOurP.h"

std::vector<objectInFile> FileOurP::parseFile(const std::ifstream& file) {
    std::vector<objectInFile> objects;
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("ID") != std::string::npos) {
            std::stringstream ss(line);
            std::string id_str;
            long long id;
            ss >> id_str >> id;

            ID object_id(id);
            std::getline(file, line);

            std::stringstream data(line);
            std::string type;
            data >> type;

            if (type == "point") {
                double x, y;
                data >> x >> y;
                point* p = new point;
                p->x = x;
                p->y = y;
                std::pair<ID,primitive*> a = {object_id, p};
                objects.push_back(objectInFile(a));
            }
            else if (type == "section") {
                double x1, y1, x2, y2;
                data >> x1 >> y1 >> x2 >> y2;
                point* p1 = new point;
                p1->x = x1;
                p1->y = y1;
                point* p2 = new point;
                p2->x = x2;
                p2->y = y2;
                section* sec = new section;
                sec->beg = p1;
                sec->end = p2;
                std::pair<ID,primitive*> a = {object_id, sec};
                objects.push_back(objectInFile(a));
            }
            else if (type == "circle") {
                double x, y, r;
                data >> x >> y >> r;
                point* center = new point;
                center->x = x;
                center->y = y;
                circle* c = new circle;
                c->center = center;
                c->R = r;
                std::pair<ID,primitive*> a = {object_id, c};
                objects.push_back(objectInFile(a));;
            }
        }
    }

    return objects;
}
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

void FileOurP::loadFromOurP(const std::string &fileName) {
    std::ifstream file(fileName + ".ourp");
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    parseFile(file);
}
const std::vector<objectInFile> &FileOurP::getObjects() const {
        return m_objects;
}
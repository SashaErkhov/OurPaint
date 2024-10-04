//
// Created by Eugene Bychkov on 28.09.2024.
//

#include "FileOurP.h"

std::vector<objectInFile> FileOurP::parseFile(std::ifstream& file) {
    std::vector<objectInFile> objects;
    std::string line;
    std::queue<std::pair<ID, primitive*>> q;

    while (std::getline(file, line)) {
        if (file.fail()) {
            throw std::runtime_error("Failed to read line from file");
        }
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
                if (q.size() == 2) {
                    objects.emplace_back(q.front());
                    q.pop();
                }
                q.push(a);

            }
            else if (type == "section") {
                if (q.size() != 2){
                    throw std::runtime_error("Invalid file format. Check instruction on rules.md");
                }

                primitive* p1 = q.front().second;
                objects.emplace_back(q.front());
                q.pop();
                primitive* p2 = q.front().second;
                objects.emplace_back(q.front());
                q.pop();
                section* sec = new section;
                sec->beg = dynamic_cast<point*>(p1);
                sec->end = dynamic_cast<point*>(p2);
                std::pair<ID,primitive*> a = {object_id, sec};
                objects.emplace_back(a);
            }
            else if (type == "circle") {
                double r;
                data >> r;
                primitive* center = q.front().second;
                objects.emplace_back(q.front());
                q.pop();
                circle* c = new circle;
                c->center = dynamic_cast<point*>(center);
                c->R = r;
                std::pair<ID,primitive*> a = {object_id, c};
                objects.emplace_back(a);;
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
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    std::vector<objectInFile> sort_objects = m_objects;
    std::sort(sort_objects.begin(), sort_objects.end(), [](const objectInFile &a, const objectInFile &b) {
        return a.to_pair().first < b.to_pair().first;
    });
    file << "Elements: {\n";
    for (const auto &obj: sort_objects) {
        file << obj.to_string() << "\n";
    }
    file << "}\n";
    //TODO::add for req
    file.close();
}

void FileOurP::loadFromOurP(const std::string &fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    m_objects = parseFile(file);
}
const std::vector<objectInFile> &FileOurP::getObjects() const {
        return m_objects;
}
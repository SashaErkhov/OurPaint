//
// Created by Eugene Bychkov on 28.09.2024.
//

#ifndef OURPAINT_OBJECTINFILE_H
#define OURPAINT_OBJECTINFILE_H

#include "../objects.h"
#include <string>

class objectInFile {
    ID id;
    primitive *s_obj;
public:
    explicit objectInFile(std::pair<ID, primitive *> &obj);


    objectInFile(const objectInFile &other);

    objectInFile(objectInFile &&other) noexcept;

    objectInFile &operator=(const objectInFile &other);

    objectInFile &operator=(objectInFile &&other) noexcept;

    std::string to_string() const;
};


#endif //OURPAINT_OBJECTINFILE_H

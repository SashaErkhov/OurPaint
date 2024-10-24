//
// Created by Eugene Bychkov on 24.10.2024.
//

#ifndef OURPAINT_REQUIREMENTINFILE_H
#define OURPAINT_REQUIREMENTINFILE_H
#include "../requirements.h"
#include <string>

class requirementInFile {
    ID id;
    RequirementData s_req;
    public:
    explicit requirementInFile(const std::pair<ID, RequirementData> &obj);

    requirementInFile(const requirementInFile &other);

    requirementInFile(requirementInFile &&other) noexcept;

    requirementInFile &operator=(const requirementInFile &other);

    requirementInFile &operator=(requirementInFile &&other) noexcept;
    [[nodiscard]] std::pair<ID, RequirementData> to_pair() const;
    [[nodiscard]] std::string to_string() const;
};


#endif //OURPAINT_REQUIREMENTINFILE_H

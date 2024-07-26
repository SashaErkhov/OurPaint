#include "paint.h"

ElementData::ElementData() {
    params = Arry<double>();
}

ID Paint::addRequirement(const RequirementData &rd) {
    c_bmpPainter = BMPpainter();
    ActionsInfo info;

    // 1
    if (rd.req == ET_POINTSECTIONDIST) {
        point* p_it = nullptr;
        section* s_it = nullptr;
        try {
            p_it = &(*(m_pointIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[0]);
            info.m_objects.addElement(rd.objects[1]);
        }
        catch (...) {
            s_it = &(*(m_sectionIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[1]);
            info.m_objects.addElement(rd.objects[0]);
        }
        if (p_it != nullptr) {
            try {
                s_it = &(*(m_sectionIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such section or point");
            }
        }
        else if (s_it != nullptr) {
            try {
                p_it = &(*(m_pointIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such point or section");
            }
        }
        IReq* requirement = new ReqPointSecDist(p_it, s_it, rd.params[0]);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        Arry<double> pointParam;
        pointParam.addElement(paramValues[0]);
        pointParam.addElement(paramValues[1]);
        Arry<double> sectionParam;
        sectionParam.addElement(paramValues[2]);
        sectionParam.addElement(paramValues[3]);
        sectionParam.addElement(paramValues[4]);
        sectionParam.addElement(paramValues[5]);
        info.m_paramsBefore.addElement(pointParam);
        info.m_paramsBefore.addElement(sectionParam);
        Arry<double> derivatives;
        int k = 0;
        for (auto it = params.begin(); it != params.end(); ++it, ++k) {
            derivatives.addElement(requirement->getDerivative(*it));
        }
        double alpha;
        double e = requirement->getError();
        //TODO for matrix
        if (m_reqStorage.getSize() == 1) {
            IReq* requirement1 = m_reqStorage.getElement(0);
            Arry<PARAMID> params1 = requirement1->getParams();
            Matrix<double> dotParam(1, 2);
            dotParam.setElement(0, 0, paramValues[0]);
            dotParam.setElement(0, 1, paramValues[1]);
            Arry<double> derivatives1;
            int k1 = 0;
            for (auto it = params1.begin(); it != params1.end(); ++it, ++k1) {
                derivatives1.addElement(requirement1->getDerivative(*it));
            }
            Matrix<double> neededDerivatives(2, 2);
            neededDerivatives.setElement(0, 0, derivatives[0]);
            neededDerivatives.setElement(0, 1, derivatives[1]);
            neededDerivatives.setElement(1, 0, derivatives1[0]);
            neededDerivatives.setElement(1, 1, derivatives1[1]);
            neededDerivatives.invMatrix();
            double e1 = requirement1->getError();
            Matrix<double> errors(2, 1);
            errors.setElement(0, 0, e);
            errors.setElement(1, 0, e1);
            while (e1 > 10e-10 && e > 10e-10) {
                dotParam = dotParam - neededDerivatives * errors;
                e1 = requirement1->getError();
                e = requirement->getError();
                errors.setElement(0, 0, e);
                errors.setElement(1, 0, e1);
            }
            s_allFigures = s_allFigures || p_it->rect();
            s_allFigures = s_allFigures || s_it->rect();
            m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
            return s_maxID.id;
        }
        while (e > 10e-2) {
            alpha = 10e-5;
            for (int i = 0; i < paramValues.getSize(); ++i) {
                paramValues[i] -= derivatives[i] * alpha;
            }
            (p_it)->x = paramValues[0];
            (p_it)->y = paramValues[1];
            (s_it)->beg->x = paramValues[2];
            (s_it)->beg->y = paramValues[3];
            (s_it)->end->x = paramValues[4];
            (s_it)->end->y = paramValues[5];
            e = requirement->getError();
        }
        pointParam[0] = paramValues[0];
        pointParam[1] = paramValues[1];

        sectionParam[0] = paramValues[2];
        sectionParam[1] = paramValues[3];
        sectionParam[2] = paramValues[4];
        sectionParam[3] = paramValues[5];

        info.m_paramsAfter.addElement(pointParam);
        info.m_paramsAfter.addElement(sectionParam);
        c_undoRedo.add(info);
        s_allFigures = s_allFigures || p_it->rect();
        s_allFigures = s_allFigures || s_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 2
    if (rd.req == ET_POINTONSECTION) {
        point* p_it = nullptr;
        section* s_it = nullptr;
        try {
            p_it = &(*(m_pointIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[0]);
            info.m_objects.addElement(rd.objects[1]);
        }
        catch (...) {
            s_it = &(*(m_sectionIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[1]);
            info.m_objects.addElement(rd.objects[0]);
        }
        if (p_it != nullptr) {
            try {
                s_it = &(*(m_sectionIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such section or point");
            }
        }
        else if (s_it != nullptr) {
            try {
                p_it = &(*(m_pointIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such point or section");
            }
        }
        IReq* requirement = new ReqPointOnSec(p_it, s_it);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        Arry<double> pointParam;
        pointParam.addElement(paramValues[0]);
        pointParam.addElement(paramValues[1]);
        Arry<double> sectionParam;
        sectionParam.addElement(paramValues[2]);
        sectionParam.addElement(paramValues[3]);
        sectionParam.addElement(paramValues[4]);
        sectionParam.addElement(paramValues[5]);
        info.m_paramsBefore.addElement(pointParam);
        info.m_paramsBefore.addElement(sectionParam);
        Arry<double> derivatives;
        int k = 0;
        for (auto it = params.begin(); it != params.end(); ++it, ++k) {
            derivatives.addElement(requirement->getDerivative(*it));
        }
        double alpha = 10e-5;
        double e = requirement->getError();
        while (e > 10e-2) {
            for (int i = 0; i < paramValues.getSize(); ++i) {
                paramValues[i] -= derivatives[i] * alpha;
            }
            (p_it)->x = paramValues[0];
            (p_it)->y = paramValues[1];
            (s_it)->beg->x = paramValues[2];
            (s_it)->beg->y = paramValues[3];
            (s_it)->end->x = paramValues[4];
            (s_it)->end->y = paramValues[5];
            e = requirement->getError();
        }
        pointParam[0] = paramValues[0];
        pointParam[1] = paramValues[1];

        sectionParam[0] = paramValues[2];
        sectionParam[1] = paramValues[3];
        sectionParam[2] = paramValues[4];
        sectionParam[3] = paramValues[5];

        info.m_paramsAfter.addElement(pointParam);
        info.m_paramsAfter.addElement(sectionParam);
        c_undoRedo.add(info);
        s_allFigures = s_allFigures || p_it->rect();
        s_allFigures = s_allFigures || s_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 3
    if (rd.req == ET_POINTPOINTDIST) {
        point* p1_it = nullptr;
        point* p2_it = nullptr;
        try {
            p1_it = &(*(m_pointIDs[rd.objects[0]]));
            p2_it = &(*(m_pointIDs[rd.objects[1]]));
            info.m_objects.addElement(rd.objects[0]);
            info.m_objects.addElement(rd.objects[1]);
        }
        catch (...) {
            throw std::invalid_argument("No such points");
        }
        IReq* requirement = new ReqPointPointDist(p1_it, p2_it, rd.params[0]);
        Arry<PARAMID> params;
        params = requirement->getParams();
        Arry<double> values;
        values.addElement(p1_it->x);
        values.addElement(p1_it->y);
        values.addElement(p2_it->x);
        values.addElement(p2_it->y);

        Arry<double> paramP1;
        paramP1.addElement(values[0]);
        paramP1.addElement(values[1]);
        Arry<double> paramP2;
        paramP2.addElement(values[2]);
        paramP2.addElement(values[3]);
        info.m_paramsBefore.addElement(paramP1);
        info.m_paramsBefore.addElement(paramP2);
        Arry<double> derivatives;
        int k = 0;
        for (auto it = params.begin(); it != params.end(); ++it, ++k) {
            derivatives.addElement(requirement->getDerivative(*it));
        }
        double alpha = 10e-6;
        double e = requirement->getError();
        while (e > 10e-5) {
            for (int i = 0; i < values.getSize(); ++i) {
                values[i] -= derivatives[i] * alpha;
            }
            (p1_it)->x = values[0];
            (p1_it)->y = values[1];
            (p2_it)->x = values[2];
            (p2_it)->y = values[3];
            e = requirement->getError();
        }
        paramP1[0] = values[0];
        paramP1[1] = values[1];
        paramP2[0] = values[2];
        paramP2[1] = values[3];
        info.m_paramsAfter.addElement(paramP1);
        info.m_paramsAfter.addElement(paramP2);
        c_undoRedo.add(info);
        s_allFigures = s_allFigures || p1_it->rect();
        s_allFigures = s_allFigures || p2_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 4
    if (rd.req == ET_POINTONPOINT) {
        point* p1_it = nullptr;
        point* p2_it = nullptr;
        try {
            p1_it = &(*(m_pointIDs[rd.objects[0]]));
            p2_it = &(*(m_pointIDs[rd.objects[1]]));
            info.m_objects.addElement(rd.objects[0]);
            info.m_objects.addElement(rd.objects[1]);
        }
        catch (...) {
            throw std::invalid_argument("No such points");
        }
        IReq* requirement = new ReqPointOnPoint(p1_it, p2_it);
        Arry<PARAMID> params;
        params = requirement->getParams();
        Arry<double> values;
        values.addElement(p1_it->x);
        values.addElement(p1_it->y);
        values.addElement(p2_it->x);
        values.addElement(p2_it->y);
        Arry<double> paramP1;
        paramP1.addElement(values[0]);
        paramP1.addElement(values[1]);
        Arry<double> paramP2;
        paramP2.addElement(values[2]);
        paramP2.addElement(values[3]);
        info.m_paramsBefore.addElement(paramP1);
        info.m_paramsBefore.addElement(paramP2);
        Arry<double> derivatives;
        int k = 0;
        for (auto it = params.begin(); it != params.end(); ++it, ++k) {
            derivatives.addElement(requirement->getDerivative(*it));
        }
        double alpha = 10e-6;
        double e = requirement->getError();
        while (e > 10e-5) {
            for (int i = 0; i < values.getSize(); ++i) {
                values[i] -= derivatives[i] * alpha;
            }
            (p1_it)->x = values[0];
            (p1_it)->y = values[1];
            (p2_it)->x = values[2];
            (p2_it)->y = values[3];
            e = requirement->getError();
        }
        paramP1[0] = values[0];
        paramP1[1] = values[1];
        paramP2[0] = values[2];
        paramP2[1] = values[3];
        info.m_paramsAfter.addElement(paramP1);
        info.m_paramsAfter.addElement(paramP2);
        c_undoRedo.add(info);
        s_allFigures = s_allFigures || p1_it->rect();
        s_allFigures = s_allFigures || p2_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 5
    if (rd.req == ET_SECTIONCIRCLEDIST) {
        circle* c_it = nullptr;
        section* s_it = nullptr;
        try {
            c_it = &(*(m_circleIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[0]);
            info.m_objects.addElement(rd.objects[1]);
        }
        catch (...) {
            s_it = &(*(m_sectionIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[1]);
            info.m_objects.addElement(rd.objects[0]);
        }
        if (c_it != nullptr) {
            try {
                s_it = &(*(m_sectionIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such section or circle");
            }
        }
        else if (s_it != nullptr) {
            try {
                c_it = &(*(m_circleIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such circle or section");
            }
        }
        IReq* requirement = new ReqSecCircleDist(s_it, c_it, rd.params[0]);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        Arry<double> derivatives;

        Arry<double> circleParam;
        circleParam.addElement(paramValues[0]);
        circleParam.addElement(paramValues[1]);
        circleParam.addElement(paramValues[2]);
        Arry<double> sectionParam;
        sectionParam.addElement(paramValues[3]);
        sectionParam.addElement(paramValues[4]);
        sectionParam.addElement(paramValues[5]);
        sectionParam.addElement(paramValues[6]);
        info.m_paramsBefore.addElement(circleParam);
        info.m_paramsBefore.addElement(sectionParam);

        int k = 0;
        for (auto it = params.begin(); it != params.end(); ++it, ++k) {
            derivatives.addElement(requirement->getDerivative(*it));
        }
        double alpha = 10e-5;
        double e = requirement->getError();
        while (e > 10e-2) {
            for (int i = 0; i < paramValues.getSize(); ++i) {
                paramValues[i] -= derivatives[i] * alpha;
            }
            (c_it)->center->x = paramValues[0];
            (c_it)->center->y = paramValues[1];
            (c_it)->R      = paramValues[2];
            (s_it)->beg->x = paramValues[3];
            (s_it)->beg->y = paramValues[4];
            (s_it)->end->x = paramValues[5];
            (s_it)->end->y = paramValues[6];
            e = requirement->getError();
        }


        circleParam[0] = paramValues[0];
        circleParam[1] = paramValues[1];
        circleParam[2] = paramValues[2];

        sectionParam[0] = paramValues[3];
        sectionParam[1] = paramValues[4];
        sectionParam[2] = paramValues[5];
        sectionParam[3] = paramValues[6];
        info.m_paramsAfter.addElement(circleParam);
        info.m_paramsAfter.addElement(sectionParam);
        c_undoRedo.add(info);

        s_allFigures = s_allFigures || c_it->rect();
        s_allFigures = s_allFigures || s_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 6
    if (rd.req == ET_SECTIONONCIRCLE) {
        circle* c_it = nullptr;
        section* s_it = nullptr;
        try {
            c_it = &(*(m_circleIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[0]);
            info.m_objects.addElement(rd.objects[1]);
        }
        catch (...) {
            s_it = &(*(m_sectionIDs[rd.objects[0]]));
            info.m_objects.addElement(rd.objects[1]);
            info.m_objects.addElement(rd.objects[0]);
        }
        if (c_it != nullptr) {
            try {
                s_it = &(*(m_sectionIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such section or circle");
            }
        }
        else if (s_it != nullptr) {
            try {
                c_it = &(*(m_circleIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such circle or section");
            }
        }
        IReq* requirement = new ReqSecOnCircle(s_it, c_it);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }

        Arry<double> circleParam;
        circleParam.addElement(paramValues[0]);
        circleParam.addElement(paramValues[1]);
        circleParam.addElement(paramValues[2]);
        Arry<double> sectionParam;
        sectionParam.addElement(paramValues[3]);
        sectionParam.addElement(paramValues[4]);
        sectionParam.addElement(paramValues[5]);
        sectionParam.addElement(paramValues[6]);
        info.m_paramsBefore.addElement(circleParam);
        info.m_paramsBefore.addElement(sectionParam);

        Arry<double> derivatives;
        for (const auto& it : params) {
            derivatives.addElement(requirement->getDerivative(it));
        }
        double alpha = 0.01;
        double e = requirement->getError();
        while (e > 10e-2) {
            for (int i = 0; i < paramValues.getSize(); ++i) {
                paramValues[i] -= derivatives[i] * alpha;
            }
            (c_it)->center->x = paramValues[0];
            (c_it)->center->y = paramValues[1];
            (c_it)->R      = paramValues[2];
            (s_it)->beg->x = paramValues[3];
            (s_it)->beg->y = paramValues[4];
            (s_it)->end->x = paramValues[5];
            (s_it)->end->y = paramValues[6];
            e = requirement->getError();
        }

        circleParam[0] = paramValues[0];
        circleParam[1] = paramValues[1];
        circleParam[2] = paramValues[2];

        sectionParam[0] = paramValues[3];
        sectionParam[1] = paramValues[4];
        sectionParam[2] = paramValues[5];
        sectionParam[3] = paramValues[6];
        info.m_paramsAfter.addElement(circleParam);
        info.m_paramsAfter.addElement(sectionParam);
        c_undoRedo.add(info);

        s_allFigures = s_allFigures || c_it->rect();
        s_allFigures = s_allFigures || s_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 7
    if (rd.req == ET_SECTIONINCIRCLE) {
        circle* c_it = nullptr;
        section* s_it = nullptr;
        try {
            c_it = &(*(m_circleIDs[rd.objects[0]]));
        }
        catch (...) {
            s_it = &(*(m_sectionIDs[rd.objects[0]]));
        }
        if (c_it != nullptr) {
            try {
                s_it = &(*(m_sectionIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such section or circle");
            }
        }
        else if (s_it != nullptr) {
            try {
                c_it = &(*(m_circleIDs[rd.objects[1]]));
            }
            catch (...) {
                throw std::invalid_argument("No such circle or section");
            }
        }
        IReq* requirement = new ReqSecInCircle(s_it, c_it);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> derivatives;
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize() ; i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsBefore.addElement(paramValues);

        const double alpha = 0.01;
        double e = requirement->getError();
        while (e > 0.001) {
            for (auto it = params.begin(); it != params.end(); ++it) {
                derivatives.addElement(requirement->getDerivative(*it));
            }
            (c_it)->center->x -= derivatives[0] * alpha;
            (c_it)->center->y -= derivatives[1] * alpha;
            (c_it)->R         -= derivatives[2] * alpha;
            (s_it)->beg->x    -= derivatives[3] * alpha;
            (s_it)->beg->y    -= derivatives[4] * alpha;
            (s_it)->end->x    -= derivatives[5] * alpha;
            (s_it)->end->y    -= derivatives[6] * alpha;
            e = requirement->getError();
        }

        params = requirement->getParams();
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsAfter.addElement(paramValues);
        c_undoRedo.add(info);

        s_allFigures = s_allFigures || c_it->rect();
        s_allFigures = s_allFigures || s_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 8
    if (rd.req == ET_SECTIONSECTIONPARALLEL) {
        section* s1_it = nullptr;
        section* s2_it = nullptr;

        try
        {
            s1_it = &(*(m_sectionIDs[rd.objects[0]]));
            s2_it = &(*(m_sectionIDs[rd.objects[1]]));
        }
        catch (...)
        {
            throw std::invalid_argument("No such section");
        }

        IReq* requirement = new ReqSecSecParallel(s1_it, s2_it);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsBefore.addElement(paramValues);
        Arry<double> derivatives;

        const double alpha = 0.0001;
        double e = requirement->getError();
        while (e > 0.1) {
            for (auto it = params.begin(); it != params.end(); ++it) {
                derivatives.addElement(requirement->getDerivative(*it));
            }
            (s1_it)->beg->x -= derivatives[0] * alpha;
            (s1_it)->beg->y -= derivatives[1] * alpha;
            (s1_it)->end->x -= derivatives[2] * alpha;
            (s1_it)->end->y -= derivatives[3] * alpha;
            (s2_it)->beg->x -= derivatives[4] * alpha;
            (s2_it)->beg->y -= derivatives[5] * alpha;
            (s2_it)->end->x -= derivatives[6] * alpha;
            (s2_it)->end->y -= derivatives[7] * alpha;
            e = requirement->getError();
        }

        params = requirement->getParams();
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsAfter.addElement(paramValues);
        c_undoRedo.add(info);

        s_allFigures = s_allFigures || s1_it->rect();
        s_allFigures = s_allFigures || s2_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 9
    if (rd.req == ET_SECTIONSECTIONPERPENDICULAR) {
        section* s1_it = nullptr;
        section* s2_it = nullptr;

        try
        {
            s1_it = &(*(m_sectionIDs[rd.objects[0]]));
            s2_it = &(*(m_sectionIDs[rd.objects[1]]));
        }
        catch (...)
        {
            throw std::invalid_argument("No such section");
        }

        IReq* requirement = new ReqSecSecPerpendicular(s1_it, s2_it);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsBefore.addElement(paramValues);
        Arry<double> derivatives;

        const double alpha = 0.0001;
        double e = requirement->getError();
        while (e > 0.1) {
            for (auto it = params.begin(); it != params.end(); ++it) {
                derivatives.addElement(requirement->getDerivative(*it));
            }
            (s1_it)->beg->x -= derivatives[0] * alpha;
            (s1_it)->beg->y -= derivatives[1] * alpha;
            (s1_it)->end->x -= derivatives[2] * alpha;
            (s1_it)->end->y -= derivatives[3] * alpha;
            (s2_it)->beg->x -= derivatives[4] * alpha;
            (s2_it)->beg->y -= derivatives[5] * alpha;
            (s2_it)->end->x -= derivatives[6] * alpha;
            (s2_it)->end->y -= derivatives[7] * alpha;
            e = requirement->getError();
        }

        params = requirement->getParams();
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsAfter.addElement(paramValues);
        c_undoRedo.add(info);

        s_allFigures = s_allFigures || s1_it->rect();
        s_allFigures = s_allFigures || s2_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }



    // 10
    if (rd.req == ET_SECTIONSECTIONANGEL) {
        section* s1_it = nullptr;
        section* s2_it = nullptr;

        try
        {
            s1_it = &(*(m_sectionIDs[rd.objects[0]]));
            s2_it = &(*(m_sectionIDs[rd.objects[1]]));
        }
        catch (...)
        {
            throw std::invalid_argument("No such section");
        }

        IReq* requirement = new ReqSecSecAngel(s1_it, s2_it, rd.params[0]);
        Arry<PARAMID> params = requirement->getParams();
        Arry<double> paramValues;
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsBefore.addElement(paramValues);
        Arry<double> derivatives;
        for (auto it = params.begin(); it != params.end(); ++it) {
            derivatives.addElement(requirement->getDerivative(*it));
        }

        const double alpha = 0.001;
        double e = requirement->getError();
        while (e > 0.001) {
            (s1_it)->beg->x -= derivatives[0] * alpha;
            (s1_it)->beg->y -= derivatives[1] * alpha;
            (s1_it)->end->x -= derivatives[2] * alpha;
            (s1_it)->end->y -= derivatives[3] * alpha;
            (s2_it)->beg->x -= derivatives[4] * alpha;
            (s2_it)->beg->y -= derivatives[5] * alpha;
            (s2_it)->end->x -= derivatives[6] * alpha;
            (s2_it)->end->y -= derivatives[7] * alpha;
            e = requirement->getError();
        }

        params = requirement->getParams();
        for (size_t i = 0; i < params.getSize(); i++)
        {
            paramValues.addElement(*params[i]);
        }
        info.m_paramsAfter.addElement(paramValues);
        c_undoRedo.add(info);

        s_allFigures = s_allFigures || s1_it->rect();
        s_allFigures = s_allFigures || s2_it->rect();
        m_reqIDs[++s_maxID.id] = m_reqStorage.addElement(requirement);
        return s_maxID.id;
    }

    return ID{-1};
}

ID Paint::addElement(const ElementData &ed) {
    ActionsInfo info;
    if (ed.et == ET_POINT) {
        point tmp;
        tmp.x = ed.params[0];
        tmp.y = ed.params[1];
        s_allFigures = s_allFigures || tmp.rect();
        m_pointIDs[++s_maxID.id] = m_pointStorage.addElement(tmp);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(ed.params);
        c_undoRedo.add(info);
        return s_maxID;
    }
    if (ed.et == ET_SECTION) {
        point tmp1;
        tmp1.x = ed.params[0];
        tmp1.y = ed.params[1];
        auto beg = m_pointStorage.addElement(tmp1);
        m_pointIDs[++s_maxID.id] = beg;
        Arry<double> params1;
        params1.addElement(ed.params[0]);
        params1.addElement(ed.params[1]);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(params1);
        point tmp2;
        tmp2.x = ed.params[2];
        tmp2.y = ed.params[3];
        auto end = m_pointStorage.addElement(tmp2);
        m_pointIDs[++s_maxID.id] = end;
        Arry<double> params2;
        params2.addElement(ed.params[2]);
        params2.addElement(ed.params[3]);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(params2);
        section tmp;
        tmp.beg = &(*beg);
        tmp.end = &(*end);
        s_allFigures = s_allFigures || tmp.rect();
        m_sectionIDs[++s_maxID.id] = m_sectionStorage.addElement(tmp);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(ed.params);
        c_undoRedo.add(info);
        return s_maxID;
    }
    if (ed.et == ET_CIRCLE) {
        point center;
        center.x = ed.params[0];
        center.y = ed.params[1];
        Arry<double> params1;
        params1.addElement(ed.params[0]);
        params1.addElement(ed.params[1]);
        info.m_paramsAfter.addElement(params1);
        auto cent = m_pointStorage.addElement(center);
        m_pointIDs[++s_maxID.id] = cent;
        info.m_objects.addElement(s_maxID);
        circle tmp;
        tmp.center = &(*cent);
        tmp.R = ed.params[2];
        s_allFigures = s_allFigures || tmp.rect();
        m_circleIDs[++s_maxID.id] = m_circleStorage.addElement(tmp);
        info.m_objects.addElement(s_maxID);
        params1.addElement(ed.params[2]);
        info.m_paramsAfter.addElement(params1);
        c_undoRedo.add(info);
        return s_maxID;
    }
    return ID{-1};
}

ElementData Paint::getElementInfo(ID id) {
    ElementData result;

    try {
        auto p = m_pointIDs[id];
        result.et = ET_POINT;
        result.params.addElement((*p).x);
        result.params.addElement((*p).y);
    }
    catch (const std::runtime_error &) {
        try {
            auto sec = m_sectionIDs[id];
            result.et = ET_SECTION;
            result.params.addElement((*sec).beg->x);
            result.params.addElement((*sec).beg->y);
            result.params.addElement((*sec).end->x);
            result.params.addElement((*sec).end->y);
        }
        catch (const std::runtime_error &) {
            auto circ = m_circleIDs[id];
            result.et = ET_CIRCLE;
            result.params.addElement((*circ).center->x);
            result.params.addElement((*circ).center->y);
            result.params.addElement((*circ).R);
        }
    }

    return result;
}

RequirementData Paint::getRequirementInfo(ID id) {
    RequirementData result;
    auto req = m_reqIDs[id];
    Arry<PARAMID> paramIDs = (*req)->getParams();
    Arry<double> params;
    for (auto paramID = paramIDs.begin(); paramID != paramIDs.end(); ++paramID) {
        params.addElement(*(*paramID));
    }
    result.params = params;
    return result;
}

void Paint::paint() {
    c_bmpPainter.changeSize(s_allFigures);
    for (auto point = m_pointStorage.begin(); point != m_pointStorage.end(); ++point) {
        c_bmpPainter.drawPoint(*point, false);
    }
    for (auto circle = m_circleStorage.begin(); circle != m_circleStorage.end(); ++circle) {
        c_bmpPainter.drawCircle(*circle, false);
    }
    for (auto section = m_sectionStorage.begin(); section != m_sectionStorage.end(); ++section) {
        c_bmpPainter.drawSection(*section, false);
    }
}

/*
void Paint::saveToFile(const char* file) {
    std::ofstream fout;
    fout.open(file);
    if (!(fout.is_open())) {
        throw "We can't open file";
    }
    fout << m_pointStorage.getSize();
    point prm;
    for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end(); ++pos){
        fout<<(*pos).first.id();
        prm=*((*pos).second);
        fout<<prm.x;
        fout<<prm.y;
    }
    fout << "\n";
    fout << m_sectionStorage.getSize();
    section sct;
    bool end=false;
    for (auto pos=m_sectionIDs.begin(); pos!=m_sectionIDs.end(); ++pos) {
        end=false;
        fout<<(*pos).first().id();
        sct=*((*pos).second());
        for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end() && !(end); ++pos){
            if(&(*((*pos).second))==sct.beg){
                fout<<(*pos).first.id();
                end=true;
            }
        }
        for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end() && !(end); ++pos){
            if(&(*((*pos).second))==sct.end){
                fout<<(*pos).first.id();
                end=true;
            }
        }
    }
    fout << "\n";
    fout << m_circleStorage.getSize();
    circle crc;
    bool end=false;
    for (auto pos=m_sectionIDs.begin(); pos!=m_sectionIDs.end(); ++pos) {
        end=false;
        fout<<(*pos).first.id();
        crc=*((*pos).second);
        for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end() && !(end); ++pos){
            if(&(*((*pos).second))==crc.center){
                fout<<(*pos).first.id();
                end=true;
            }
        }
        fout<<crc.R;
    }
    fout.close();
}


void Paint::loadFromFile(const char* filename) {
    std::ifstream files;
    files.open(filename);
    if (!(files.is_open())) {
        throw "We can't open file";
    }
    m_pointIDs = Assoc<ID, List<point>::iterator>();
    m_sectionIDs = Assoc<ID, List<section>::iterator>();
    m_circleIDs = Assoc<ID, List<circle>::iterator>();
    size_t size=0;
    files >> size;
    point need;
    ID id;
    s_maxID = 0;
    List<point>::iterator point_iter;
    m_pointStorage = List<point>(0);
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> need;//нужно создать ввод для таких элементов
        point_iter =m_pointStorage.addElement(need);
        m_pointIDs.addPair(id, point_iter);
    }
    files >> size;
    section work;
    List<section>::iterator section_iter;
    m_sectionStorage = List<section>(0);
    ID beg_section;
    ID end_section;
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> beg_section;
        files >> end_section;
        work.beg=&(*m_pointIDs.findByKey(beg_section));
        work.end=&(*m_pointIDs.findByKey(end_section));
        section_iter = m_sectionStorage.addElement(work);
        m_sectionIDs.addPair(id, section_iter);
    }
    files >> size;
    circle worker;
    List<circle>::iterator circle_iter;
    m_circleStorage = List<circle>(0);
    ID center;
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> center;
        worker.center=&(*m_pointIDs.findByKey(center));
        files >> worker.R;
        circle_iter = m_circleStorage.addElement(worker);
        m_circleIDs.addPair(id, circle_iter);
    }
}
*/

void Paint::exportToBMP(const char *file) {
    paint();
    try {
        c_bmpPainter.saveBMP(file);
    }
    catch (...) {
        throw std::invalid_argument("Can not opened file!");
    }
}

/*
void Paint::makeMySectionOrt(const ElementData& ed, ElementData& changing){
    if (ed.et != ET_SECTION or changing.et != ET_SECTION) {
        throw "Some of the elements is not section!";
    }
    //изменяем changing так, чтобы он был ортогонален с ed(cкорее всего, поворотом одной из точек)
}
void Paint::makeMySectionEqual(const ElementData& ed, ElementData& changing) {
    if (ed.et != ET_SECTION or changing.et != ET_SECTION) {
        throw "Some of the elements is not section!";
    }
    //изменяем changing так, чтобы его длина была равна длине ed (cкорее всего, поворотом одной из точек)
    //например поменяем Y второй точки
    double len2 = pow(ed.point1.x - ed.point2.x, 2) + pow(ed.point1.x - ed.point2.x, 2)
    changing.point1.y = sqrt(len2-pow(changing.point1.x -changing.point2.x, 2))+ changing.point1.y
}
void Paint::makeMySectionParallel(const ElementData& ed, ElementData& changing) {
    if (ed.et != ET_SECTION or changing.et != ET_SECTION) {
        throw "Some of the elements is not section!";
    }
    //изменяем changing так, чтобы он стал параллелен ed
}
void Paint::makeMySectionVertical(ElementData& changing) {
    if (changing.et != ET_SECTION) {
        throw "The element is not section!";
    }
    //отрезок становится строго вертикальным
    changing.point2.x=changing.point1.x; - чтобы были равны X координаты точек
}
void Paint::makeMySectionHorizontal(ElementData& changing) {
    if (changing.et != ET_SECTION) {
        throw "The element is not section!";
    }
    //отрезок становится строго горизонтальным2
    changing.point2.y=changing.point1.y; - чтобы были равны Y координаты точек
}
void Paint::makeMyCircleEqual(const ElementData& ed, ElementData& changing) {
    if (ed.et != ET_CIRCLE or changing.et != ET_CIRCLE) {
        throw "Some of the elements is not Circle!";
    }
    //окружности становятся одинакового размера
    changing.radius = ed.radius;
}
*/


void Paint::changeBMP(const BMPfile &file) {
    c_bmpPainter = BMPpainter(file);
}

void Paint::changeBMP(const char *filename) {
    c_bmpPainter = BMPpainter(BMPfile(filename));
}


void Paint::deleteRequirement(ID req) {
    try {
        m_reqStorage.remove(m_reqIDs[req]);
    } catch (...) {
        std::cout << "no requirement with ID " << req.id << std::endl;
    }
}

void Paint::undo() {
    ActionsInfo info = c_undoRedo.undo();
    point *p = nullptr;
    section *s = nullptr;
    circle *c = nullptr;
    if (info.m_paramsBefore.getSize() == 0) {
        for (int i = 0; i < info.m_objects.getSize(); ++i) {
            if (m_pointIDs.contains(info.m_objects[i])) {
                m_pointStorage.remove(m_pointIDs[info.m_objects[i]]);
            }else if (m_sectionIDs.contains(info.m_objects[i])) {
                m_sectionStorage.remove(m_sectionIDs[info.m_objects[i]]);
            }else if (m_circleIDs.contains(info.m_objects[i])){
                m_circleStorage.remove(m_circleIDs[info.m_objects[i]]);
            } else {
                std::cout << "No ID to undo" << std::endl;
                break;
            }
        }
        return;
    }
    for (int i = 0; i < info.m_objects.getSize(); ++i) {
        try {
            p = &(*m_pointIDs[info.m_objects[i]]);
            p->x = info.m_paramsBefore[i][0];
            p->y = info.m_paramsBefore[i][1];
        } catch (...) {
            try {
                s = &(*m_sectionIDs[info.m_objects[i]]);
                s->beg->x = info.m_paramsBefore[i][0];
                s->beg->y = info.m_paramsBefore[i][1];
                s->end->x = info.m_paramsBefore[i][2];
                s->end->y = info.m_paramsBefore[i][3];
            } catch (...) {
                try {
                    c = &(*m_circleIDs[info.m_objects[i]]);
                    c->center->x = info.m_paramsBefore[i][0];
                    c->center->y = info.m_paramsBefore[i][1];
                } catch (...) {
                    std::cout << "No ID to undo" << std::endl;
                    break;
                }
            }
        }
    }
}

void Paint::redo() {
    ActionsInfo info = c_undoRedo.redo();
    point *p = nullptr;
    section *s = nullptr;
    circle *c = nullptr;
    if (info.m_paramsBefore.getSize() == 0) {
        if (info.m_objects.getSize() == 3){
            point beg;
            beg.x = info.m_paramsAfter[0][0];
            beg.y = info.m_paramsAfter[0][1];
            auto p1 = m_pointStorage.addElement(beg);
            m_pointIDs[info.m_objects[0]] = p1;
            point end;
            end.x = info.m_paramsAfter[1][0];
            end.y = info.m_paramsAfter[1][1];
            auto p2 = m_pointStorage.addElement(end);
            m_pointIDs[info.m_objects[1]] = p2;
            section sec;
            sec.beg = &(*(p1));
            sec.end = &(*(p2));
            m_sectionIDs[info.m_objects[2]] = m_sectionStorage.addElement(sec);
            s_allFigures = s_allFigures || sec.rect();
        }else if (info.m_objects.getSize() == 2){
            point center;
            center.x = info.m_paramsAfter[0][0];
            center.y = info.m_paramsAfter[0][1];
            circle circ;
            auto p1 = m_pointStorage.addElement(center);
            m_pointIDs[info.m_objects[0]] = p1;
            circ.center = &(*(p1));
            circ.R = info.m_paramsAfter[1][2];
            m_circleIDs[info.m_objects[1]] = m_circleStorage.addElement(circ);
            s_allFigures = s_allFigures || circ.rect();
        } else if (info.m_objects.getSize() == 1){
            point pt;
            pt.x = info.m_paramsAfter[0][0];
            pt.y = info.m_paramsAfter[0][1];
            m_pointIDs[info.m_objects[0]] = m_pointStorage.addElement(pt);
            s_allFigures = s_allFigures || pt.rect();
        }
        return;
    }
    for (int i = 0; i < info.m_objects.getSize(); ++i) {
        try {
            p = &(*m_pointIDs[info.m_objects[i]]);
            p->x = info.m_paramsAfter[i][0];
            p->y = info.m_paramsAfter[i][1];
        } catch (...) {
            try {
                s = &(*m_sectionIDs[info.m_objects[i]]);
                s->beg->x = info.m_paramsAfter[i][0];
                s->beg->y = info.m_paramsAfter[i][1];
                s->end->x = info.m_paramsAfter[i][2];
                s->end->y = info.m_paramsAfter[i][3];
            } catch (...) {
                try {
                    c = &(*m_circleIDs[info.m_objects[i]]);
                    c->center->x = info.m_paramsAfter[i][0];
                    c->center->y = info.m_paramsAfter[i][1];
                } catch (...) {
                    std::cout << "No ID to redo" << std::endl;
                    break;
                }
            }
        }
    }
}

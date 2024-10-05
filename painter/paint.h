#pragma once
#ifndef PAINT_23
#define PAINT_23

#include "BMPfile.h"
#include "List.h"
#include "objects.h"
#include "BMPpainter.h"
#include "QTPainter.h"
#include "Assoc.h"
#include "../Matrix/Matrix.h"
#include "saveload/FileOurP.h"

//#include "../graph/simpleGraph.h"
#include "enums.h"
#include "requirements.h"
#include "UndoRedo.h"

#include <map>
struct ElementData {
    Element et;
    Arry<double> params;
    ElementData();
};
struct ActionsInfo{
    bool isNew;
    Arry<ID> m_objects;
    Arry<Arry<double>> m_paramsBefore;
    Arry<Arry<double>> m_paramsAfter;
};

//c_ - класс, v_- переменная, s_структура, m_ - контейнеры(списки, массивы и другие) f_ - приватный метод класса
class Paint {
    //In the graph, the edges are represented by requirements and the vertices are represented by objects.
    //Graph<ID, ID> c_graph;

    UndoRedo<ActionsInfo> c_undoRedo;
    std::map<ID, List<point>::iterator> m_pointIDs;
    std::map<ID, List<section>::iterator> m_sectionIDs;
    std::map<ID, List<circle>::iterator> m_circleIDs;
    std::map<ID, List<IReq*>::iterator> m_reqIDs;
    ID s_maxID;
    List<RequirementData> m_reqD;
    List<IReq*> m_reqStorage;
    List<point> m_pointStorage;
    List<section> m_sectionStorage;
    List<circle> m_circleStorage;
    Painter* c_bmpPainter;
    rectangle s_allFigures;
    /*
    * TODO
    * All methods (addElement, getElementInfo, etc.) must be adapted to work with the new data structure,
    * utilizing Assoc methods for adding, searching, and deleting elements.
    */

public:
    Paint(Painter* p) : s_maxID(0), m_pointIDs(),c_bmpPainter(p), m_sectionIDs(), m_circleIDs(), m_pointStorage(),
        m_sectionStorage(), m_circleStorage() {
        s_allFigures.x_1 = -10;
        s_allFigures.x_2 = 10;
        s_allFigures.y_1 = -10;
        s_allFigures.y_2 = 10;
    } //default constructor
        
    // Добавление элементов с указанием их типа и необходимого набора параметров
    ID addElement(const ElementData &ed);
    
    // Добавление требованием
    ID addRequirement(const RequirementData &rd);

    // Получение информации об объекте    
    ElementData getElementInfo(ID id);
    std::vector<std::pair<ID, ElementData>> getAllElementsInfo();
    RequirementData getRequirementInfo(ID id);

    // Сохранение данных в файл
    void saveToFile(const char *filename);
    
    // Экспорт в BMP файл
    void exportToBMP(const char *file);
    
    // Загрузка данных из файла
    void loadFromFile(const char *file);
    
    // Задает фон(изменяет filename файл)
    // void changeBMP(const BMPfile& file);
    // void changeBMP(const char* filename);

    void deleteElement(ID elem);
    void deleteRequirement(ID req);
    void clear();

    void undo();
    void redo();

    void paint();
};

#endif
//
// Created by Eugene Bychkov on 28.09.2024.
//
#include "../painter/saveload/objectInFile.h"
#include "../painter/saveload/FileOurP.h"

#include <gtest/gtest.h>
#include <sstream>
TEST(ObjectInFileTest, TestPointToString) {

    point p;
    p.x = 1.0;
    p.y = 2.0;
    std::pair<ID, primitive*> obj{ID{1}, &p};

    objectInFile objInFile(obj);

    EXPECT_EQ(objInFile.to_string(), "{\nID 1\npoint 1.000000 2.000000\n}");
}

TEST(ObjectInFileTest, TestSectionToString) {
    point p1, p2;
    p1.x = 1.0;
    p1.y = 2.0;
    p2.x = 3.0;
    p2.y = 4.0;
    section s;
    s.beg = &p1;
    s.end = &p2;
    std::pair<ID, primitive*> obj{ID{2}, &s};

    objectInFile objInFile(obj);

    EXPECT_EQ(objInFile.to_string(), "{\nID 2\nsection 1.000000 2.000000 3.000000 4.000000\n}");
}

TEST(ObjectInFileTest, TestCircleToString) {
    point center;
    center.x = 1;
    center.y = 2;
    circle c;
    c.center = &center;
    c.R = 5.0;
    std::pair<ID, primitive*> obj{ID{3}, &c};

    objectInFile objInFile(obj);

    EXPECT_EQ(objInFile.to_string(), "\nID 3\ncircle 1.000000 2.000000 5.000000\n}");
}

TEST(ObjectInFileTest, TestCopyConstructor) {
    point p;
    p.x = 1.0;
    p.y = 2.0;
    std::pair<ID, primitive*> obj{ID{4}, &p};

    objectInFile objInFile1(obj);
    objectInFile objInFile2(objInFile1);

    EXPECT_EQ(objInFile2.to_string(), objInFile1.to_string());
}

TEST(ObjectInFileTest, TestMoveConstructor) {
    point p;
    p.x = 1.0;
    p.y = 2.0;
    std::pair<ID, primitive*> obj{ID{5}, &p};

    objectInFile objInFile1(obj);
    objectInFile objInFile2(std::move(objInFile1));

    EXPECT_EQ(objInFile2.to_string(), "{\nID 5\npoint 1.000000 2.000000\n}");
}

TEST(ObjectInFileTest, TestCopyAssignmentOperator) {
    point p;
    p.x = 1.0;
    p.y = 2.0;
    std::pair<ID, primitive*> obj{ID{6}, &p};

    objectInFile objInFile1(obj);
    objectInFile objInFile2 = objInFile1;

    EXPECT_EQ(objInFile2.to_string(), objInFile1.to_string());
}

TEST(ObjectInFileTest, TestMoveAssignmentOperator) {
    point p;
    p.x = 1.0;
    p.y = 2.0;
    std::pair<ID, primitive*> obj{ID{7}, &p};

    objectInFile objInFile1(obj);
    objectInFile objInFile2 = std::move(objInFile1);

    EXPECT_EQ(objInFile2.to_string(), "{\nID 7\npoint 1.000000 2.000000\n}");
}
TEST(SaveToFileTest, TestMoveAssignmentOperator) {
    point p;
    p.x = 1.0;
    p.y = 2.0;
    std::pair<ID, primitive*> obj{ID{7}, &p};

    objectInFile objInFile1(obj);
    objectInFile objInFile2 = std::move(objInFile1);
    EXPECT_EQ(objInFile2.to_string(), "{\nID 7\npoint 1.000000 2.000000\n}");
}
std::string readFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TEST(FileOurPTest, DefaultConstructor) {
    FileOurP file;
    EXPECT_EQ(file.getObjects().size(), 0);
}
TEST(FileOurPTest, AddObject) {
    FileOurP file;
    ID id{1};
    point p;
    p.x = 0;
    p.y = 0;
    primitive* prim = &p;
    std::pair<ID, primitive*> obj(id, prim);

    file.addObject(obj);

    EXPECT_EQ(file.getObjects().size(), 1);
}

TEST(FileOurPTest, SaveToOurP) {
    FileOurP file;
    ID id{1};
    point pt;
    pt.x = 0;
    pt.y = 0;
    primitive* prim = &pt;
    std::pair<ID, primitive*> obj(id, prim);
    file.addObject(obj);

    std::string fileName = "test_file";
    file.saveToOurP(fileName);

    std::string content = readFile(fileName + ".ourp");
    std::string expectedContent = "Elements: {\n{\nID 1\npoint 0.000000 0.000000\n}\n}\n";

    EXPECT_EQ(content, expectedContent);
}
TEST(FileOurPTest, MoveConstructor) {
    FileOurP file;
    ID id{1};
    point p;
    p.x = 1;
    p.y = 1;
    primitive* prim = &p;
    std::pair<ID, primitive*> obj(id, prim);
    file.addObject(obj);

    FileOurP movedFile(std::move(file));

    EXPECT_EQ(movedFile.getObjects().size(), 1);
    EXPECT_EQ(file.getObjects().size(), 0);
}


TEST(FileOurPTest, CopyConstructor) {
    FileOurP file;
    ID id{1};
    point p;
    p.x = 1;
    p.y = 1;
    primitive* prim = &p;
    std::pair<ID, primitive*> obj(id, prim);
    file.addObject(obj);

    FileOurP copiedFile(file);

    EXPECT_EQ(copiedFile.getObjects().size(), 1);
    EXPECT_EQ(file.getObjects().size(), 1);
}

TEST(FileOurPTest, MoveAssignment) {
    FileOurP file1;
    FileOurP file2;

    ID id{2};
    point p;
    p.x = 2;
    p.y = 2;
    primitive* prim = &p;
    std::pair<ID, primitive*> obj(id, prim);
    file1.addObject(obj);

    file2 = std::move(file1);

    EXPECT_EQ(file2.getObjects().size(), 1);
    EXPECT_EQ(file1.getObjects().size(), 0);
}

TEST(FileOurPTest, CopyAssignment) {
    FileOurP file1;
    FileOurP file2;

    ID id{3};
    point p;
    p.x = 3;
    p.y = 3;
    primitive* prim = &p;
    std::pair<ID, primitive*> obj(id, prim);
    file1.addObject(obj);

    file2 = file1;

    EXPECT_EQ(file2.getObjects().size(), 1);
    EXPECT_EQ(file1.getObjects().size(), 1);
}

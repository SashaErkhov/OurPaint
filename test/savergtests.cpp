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

    EXPECT_EQ(objInFile.to_string(), "{\nID 2\nsection\n}");
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

    EXPECT_EQ(objInFile.to_string(), "\nID 3\ncircle 5.000000\n}");
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
    pt.x = 1;
    pt.y = 1;
    primitive* prim = &pt;
    std::pair<ID, primitive*> obj(id, prim);
    file.addObject(obj);
    ID id2{2};
    point beg1;
    beg1.x = 3;
    beg1.y = 4;
    std::pair<ID, primitive*> obj2(id2, prim);
    file.addObject(obj2);
    ID id3{3};
    point end1;
    end1.x = 5;
    end1.y = 6;
    std::pair<ID, primitive*> obj3(id3, prim);
    file.addObject(obj3);
    ID id4{4};
    section sec;
    sec.beg = &beg1;
    sec.end = &end1;
    primitive* prim2 = &sec;
    std::pair<ID, primitive*> obj1(id2, prim2);
    file.addObject(obj1);
    std::string fileName = "test_file";
    file.saveToOurP(fileName);

    std::string content = readFile(fileName + ".ourp");
    std::string expectedContent = "Elements: {\n{\nID 1\npoint 1.000000 1.000000\n}\n{\nID 2\npoint 1.000000 1.000000\n}\n{\nID 2\nsection\n}\n{\nID 3\npoint 1.000000 1.000000\n}\n}\n";

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
TEST(FileOurPTest, loadFromOurPTest) {
    FileOurP file;
    ID id{1};
    point pt;
    pt.x = 1;
    pt.y = 2;
    primitive* prim = &pt;
    std::pair<ID, primitive*> obj(id, prim);
    file.addObject(obj);
    ID id3{3};
    point end1;
    end1.x = 5;
    end1.y = 6;
    std::pair<ID, primitive*> obj3(id3, &end1);
    file.addObject(obj3);
    ID id2{2};
    point beg1;
    beg1.x = 3;
    beg1.y = 4;
    std::pair<ID, primitive*> obj2(id2, &beg1);
    file.addObject(obj2);
    ID id4{4};
    section sec;
    sec.beg = &beg1;
    sec.end = &end1;
    primitive* prim2 = &sec;
    std::pair<ID, primitive*> obj4(id4, prim2);
    file.addObject(obj4);
    std::string fileName = "test_file";
    file.saveToOurP(fileName);
    FileOurP fileOurP;
    fileOurP.loadFromOurP("test_file");
    ASSERT_EQ(fileOurP.getObjects().size(), 4);

    ASSERT_EQ(fileOurP.getObjects()[0].to_pair().first.id, 1);
    ASSERT_EQ(dynamic_cast<point*>(fileOurP.getObjects()[0].to_pair().second)->x, 1);
    ASSERT_EQ(dynamic_cast<point*>(fileOurP.getObjects()[0].to_pair().second)->y, 2);

    ASSERT_EQ(fileOurP.getObjects()[3].to_pair().first.id, 4);
    ASSERT_EQ(dynamic_cast<section*>(fileOurP.getObjects()[3].to_pair().second)->beg->x, 3);
    ASSERT_EQ(dynamic_cast<section*>(fileOurP.getObjects()[3].to_pair().second)->beg->y, 4);
    ASSERT_EQ(dynamic_cast<section*>(fileOurP.getObjects()[3].to_pair().second)->end->x, 5);
    ASSERT_EQ(dynamic_cast<section*>(fileOurP.getObjects()[3].to_pair().second)->end->y, 6);
}


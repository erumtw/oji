/* 
    สำหรับเก็บข้อมูลชื่อและคะแนน
    
    ผู้พัฒนา : นาย กร โรจน์รัตนปัญญา
    เขียนเมื่อ : 13-11-2021
*/

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/** 
 * สำหรับบันทึกชื่อและคะแนนเข้าด้วยกัน
 * 
 * @param string ชื่อ
 * @param int คะแนน
 */
class Score {
private:

    std::string name;
    unsigned long score;
    
public:

    Score(std::string name, unsigned long score);

    void setName(std::string name);
    std::string getName();

    void setScore(unsigned long score);
    unsigned long getScore();
};
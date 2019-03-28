#pragma once
#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
void process(std::string origin, std::string destination, std::vector<std::string>position,unsigned int num_pts = 0, std::string result = "");
std::vector<int> get_index(std::vector<std::string> header, std::vector<std::string> position);
std::vector<std::string> separate(std::string row);
std::vector<double> str2num(std::vector<std::string> row_seperated, std::vector<int> index);
//void allen(std::vector<std::vector<double>> data, unsigned int num_pts,std::string result);
//std::vector<std::vector<double>>load(std::string destination);
#endif
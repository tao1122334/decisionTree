//
// Created by 97037 on 2024/4/29.
//
#include <cmath>
#include "tree.h"
#include <tuple>
#include <unordered_set>
//计算一串数据对应的目标变量信息熵
double TreeNode::entropy(const vector<double> &labels) {
    map<double, int> counts;
    for (const double label:labels) {
        counts[label]++;
    }

    double entropy = 0.0;
    for (auto &count: counts) {
        double p = count.second / static_cast<double>(labels.size());
        entropy -= p * log2(p);
    }
    return entropy;
}

//遍历该特征的值，决定大于等于，小于等于两类，返回该特征下最应该用于区分的值以及是大是小
tuple<double, double> TreeNode::bestKey(const vector<pair<double, double>> &data, int start, int end) {
    //获得全体数据的目标变量
    vector<double> labels;
    for (const auto &item: data) {
        labels.push_back(item.second);
    }
    //获得全体数据熵
    double totalEntropy = entropy(labels);
    double maxInfoGain = -INFINITY; // 用来跟踪最大信息增益
    double bestKey = -INFINITY; // 用来记录产生最大信息增益的键
    unordered_set<double> Set;
    for (int i = start; i < end; ++i) {
        Set.insert(data[i].first);
    }
    for (double key: Set) {
        vector<double> geq,lt;
        for (int j = start; j < end; ++j) {
            cout<<data[j].first<<","<<data[j].second<<endl;
            if (data[j].first >= key) {
                geq.push_back(data[j].second);
            } else {
                lt.push_back(data[j].second);
            }
        }
        cout<<"the key val is "<<key<<endl;
        cout<<"geq: [";
        for (double num : geq) {
            cout << num << " ";
        }
        std::cout<<"]" << std::endl;
        cout<<"lt: [";
        for (double num : lt) {
            std::cout << num << " ";
        }
        std::cout<<"]" << std::endl;
        //获得区分开子集的信息增益
        double weightedEntropy = (static_cast<double>(geq.size()) / static_cast<double>(end-start)) * entropy(geq) +
                                 (static_cast<double>(lt.size()) / static_cast<double>(end-start)) * entropy(lt);
        double infoGain = totalEntropy - weightedEntropy;

        // 检查是否是当前最大的信息增益
        if (infoGain > maxInfoGain) {
            maxInfoGain = infoGain;
            bestKey = key;
        }
        cout<<"get the infogain is "<<infoGain<<endl;
    }
    cout<<"the maxInfoGain is "<<maxInfoGain<<endl;
    cout<<"the bestKey is "<<bestKey<<endl;
    return make_tuple(bestKey, maxInfoGain);
}

void TreeNode::buildTree(map<string, vector<pair<double, double>>> &src, int depth) {
    if (depth <= 0) {
        return;
    }
    double bestSelectedKey = -INFINITY;
    double bestMaxInfoGain = -INFINITY; // 用来跟踪最大信息增益
    string bestName;
    int bestStart=0;
    int bestEnd=0;
    // 遍历每一列
    for (const auto &colName: src) {
        const string &name = colName.first;  // 当前列
        auto it = this->position.find(name);
        if (it != position.end()) {
            const vector<pair<double, double>> &data = colName.second;
            const pair<int, int> &range = it->second;
            int start = range.first;
            int end = range.second;

            auto [selectedKey, maximumInfoGain] = bestKey(data, start, end);

            // 检查是否是当前最大的信息增益
            if (maximumInfoGain > bestMaxInfoGain) {
                bestSelectedKey = selectedKey;
                bestMaxInfoGain = maximumInfoGain;
                bestName = name;
                bestStart=start;
                bestEnd=end;
            }
            cout<<"after check, the feature name is "<<name<<endl;
            cout<<"its best val to div is "<<maximumInfoGain<<endl;
        }
    }
    this->feature.name = bestName;
    this->feature.val = bestSelectedKey;
    
    if (depth > 1) {
        int mid = bestStart; // 初始化为 start，将会调整为正确的中点
        for (int i = bestStart; i < bestEnd; i++) {
            if (src[bestName][i].first >= bestSelectedKey) {
                mid = i;
                break;
            }
        }
        cout<<"the feature name is "<<this->feature.name<<endl;
        cout<<"the val is "<<this->feature.val<<endl;
        cout<<"the best inf is "<<bestMaxInfoGain<<endl;
        if (bestStart<=mid){
            // 创建子节点
            auto lfeat = Feature();
            this->left = new TreeNode(lfeat, this->position);
            this->left->position[bestName]=make_pair(bestStart, mid);
            // 递归构建左右子树
            this->left->buildTree(src, depth - 1);
        }
        if (mid<=bestEnd){
            auto rfeat = Feature();
            this->right = new TreeNode(rfeat, this->position);
            this->right->position[bestName]=make_pair(mid, bestEnd);
            this->right->buildTree(src, depth - 1);
        }
    }
}



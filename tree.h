//
// Created by 97037 on 2024/4/29.
//
#include <map> // 包含 map 头文件
#include <utility>
#include <vector>
#include <string>
#include <queue>
#include <iostream>

#ifndef UNTITLED_TREE_H
#define UNTITLED_TREE_H
using namespace std;
struct Feature {
    string name;
    double val{};
    Feature()= default;
    Feature(string name, double val)
        : name(std::move(name)),val(val){}
};

// 定义结构体
// 定义决策树的节点结构
struct TreeNode {
    Feature feature;
    TreeNode *left;  // 左子节点指针
    TreeNode *right; // 右子节点指针
    map<string, pair<int, int>> position;

    TreeNode(Feature feat, const map<string, pair<int, int>> pos)
            : feature(std::move(feat)), position(pos), left(nullptr), right(nullptr) {
    }


    static tuple<double, double> bestKey(const vector<pair<double, double>> &data, int start, int end);

    void buildTree(map<string, vector<pair<double, double>>> &src, int depth);

    static double entropy(const vector<double> &labels);
};

class DecisionTree {
    TreeNode *root;
    int maxDepth;
    map<string, vector<pair<double, double>>> src;
    int classNum;

//根据列名获取数据，其中键为特征，值为目标变量
//在输入之前就需要将其进行归一化和排序
public:
    DecisionTree(map<string, vector<pair<double, double>>> &data, int depth, int numClasses)
            : src(data), maxDepth(depth), classNum(numClasses) {
        // 初始化根节点的特征
        Feature rootFeature;

        map<string, pair<int, int>> rootPosition;
        for (const auto &entry : data) {
            // 范围从0到该vector的大小减一
            rootPosition[entry.first] = {0, static_cast<int>(entry.second.size())};
        }
        root = new TreeNode(rootFeature, rootPosition);

        // 开始构建树
        root->buildTree(src, maxDepth);
    }
    void bfsTraverse() const {
        if (!root) return;
        queue<TreeNode*> queue;
        queue.push(root);

        while (!queue.empty()) {
            TreeNode *current = queue.front();
            queue.pop();

            // Process the current node, e.g., print its feature's name
            cout << "Visiting node with feature: " << current->feature.name << std::endl;

            if (current->left) {
                queue.push(current->left);
            }
            if (current->right) {
                queue.push(current->right);
            }
        }
    }

    ~DecisionTree() {
        // 递归删除所有节点，防止内存泄露
        deleteTree(root);
    }

private:
    void deleteTree(TreeNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

};

#endif //UNTITLED_TREE_H

#include "tree.h"
#include <iostream>

int main() {
    // 创建测试数据
    map<string, vector<pair<double, double>>> testData = {
            {"Feature1", {{1.0, 0.0}, {2.0, 1.0}, {3.0, 0.0}, {3.0, 0.0}, {3.0, 0.0}}},
            {"Feature2", {{1.5, 1.0}, {2.5, 0.0}, {3.5, 1.0}, {3.5, 1.0}, {3.5, 0.0}}}
    };

    // 初始化决策树
    int maxDepth = 5;
    int numClasses = 2; // 假设有两个类别
    DecisionTree tree(testData, maxDepth, numClasses);

    // BFS遍历并打印节点信息
    cout << "BFS Traversal of Decision Tree:" << endl;
    tree.bfsTraverse();

    return 0;
}
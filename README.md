# Game-2048

## 实现思路

1. [N, N]矩阵作为地图
2. [1, N*N+1]向量作为空格点映射，用于随机空格点选取及剩余格点判断等
3. 上下左右操作后的地图变换，实现较为朴素：“所有元素根据操作对齐到一端” -> “从端开始寻找相等数对并相加” -> “再做对齐操作，并清除冗余位”

## 待完善

1. 用户分数记录
2. Python重写
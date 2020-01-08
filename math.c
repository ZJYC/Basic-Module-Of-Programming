/* 重复性计算 */

import math

RawData = [141.1,141.2,141.3,141.5,141.2]

AVG = list()

def FUNC():
    global SumX
    S = 0
    N = len(RawData)
    for i in range(0,N):AVG.append(sum(RawData) / len(RawData))
    for i in range(0,N):S += math.pow(RawData[i] - AVG[i],2)
    V = math.sqrt(S/(N-1))/AVG[0]
    return V

print("重复性:%.3f%%"%(FUNC()*100))


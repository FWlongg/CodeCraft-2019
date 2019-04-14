//2019-4-14
//Author F.J.S.
#include"Datas.h"

class PanTiQi{
public:
    PanTiQi(RoadDatas &roadDatas,CrossDatas &crossDatas){
        m_roadDatas =roadDatas;
        m_crossDatas = crossDatas;
    };
    void push_back(const int &carId){
        m_carDatas.push_back(g_carDatas,carId);
    }
    void clear();
	void updateRoad();
    void panjue(Result &result,const bool &isChangeGloble);

private:
    int MAX_IN_ROAD_CAR = INF;
    CarDatas m_carDatas;
    RoadDatas m_roadDatas;
    CrossDatas m_crossDatas;
};

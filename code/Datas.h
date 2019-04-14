//2019-4-14
//Author F.J.S.
#ifndef DATA_H
#define DATA_H

#include "iostream"
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <unordered_map>  
#include <algorithm>
#include <map>
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <set>
using namespace std;

#ifndef U_INT
#define U_INT unsigned int
#endif
extern double g_v_a;//参数A
//最大参数
static const int  MaxTime = 5000;//顶点数量
static const int  MaxSize = 180;//顶点数量
static const int  MaxSpeed = 180;//顶点数量
static const int  INF = 0x8FFFFF;

//test
///判决器
const int NO_ROAD_WAITING_STATE = 1;//没上路等待
const int IN_ROAD_START_STATE = 2;//时间片的起始状态
const int IN_ROAD_WAITING_STATE = 4;//当前时间片等待
const int IN_ROAD_OUTROAD_STATE = 8;//在路上等待转弯
const int IN_ROAD_END_STATE = 16;//当前时间片终止
const int NO_ROAD_ARRIVED_STATE = 4096;//到达终点

const int GO_STRAIGHT = 4;//直行
const int TURN_LEFT = 2;//左转
const int TURN_RIGHT = 1;;//右转
struct Result
{
	bool isFail=true;
	int lastTime=0;
	union 
	{
		struct{
			int scheduleT;
			int T_or;
			int T_pri;
		};
		int T3[3];
	};
	
	
};
unsigned int myrandom();
class Cross
{
public:
	int roads[4];//连接的四条道路 -1为不连接
	vector<int> orderConnectRoads;//路口连接道路的id升序
	map<int, int> roadIndex;//<road,index>
	int id;//程序识别的index
	int trueid;//原始Id
};
//CrossDatas-----------------------
class CrossDatas{
public:
	const int &getIndex(const int &trueid){
		if (m_trueidToIndex.count(trueid) != 0){
			return m_trueidToIndex[trueid];
		}
		else{
			cout << "CrossDatas Something Wrong!" << endl;
		}
		getchar();
	};
	const int &getTrueId(const int &index){
		if (index>=0 && index < m_cross.size()){
			return m_cross[index].id;
		}
		else{
			cout << "CrossDatas Something Wrong!" << endl;
		}
		getchar();
	};
	CrossDatas(){};
	void open(string &crossPath);
	~CrossDatas(){};
	Cross& operator[](const int & index){ return m_cross[index]; };
	const int size(){ return m_cross.size(); };
	Cross& findById(const int &index){
		return m_cross[index];
		//cout << "CrossDatas findById Error" << endl;
	};
	vector<Cross> m_cross;
	unordered_map<int, int> m_trueidToIndex;//trueid到Index的map
};
extern CrossDatas g_crossDatas;

class Car
{
public:
	//车辆的输入信息
	int id;
	int index;
	int truefrom;//这个是cross的真实Id
	int trueto;//这个是cross的真实Id
	int speed;
	int orgPlanTime;
	int priority;
	int preset;
	U_INT sortFlag;//排序
	const int&  getfrom(){
		return g_crossDatas.getIndex(this->truefrom);
	}
	const int&  getto(){
		return g_crossDatas.getIndex(this->trueto);
	}
	//路径信息
	int planTime;
	vector<pair<int,int>> path_crossId;	// vi到vj的最短路径 //用了16 位 前8位from  后8位to
	vector<int> path_roadId;
	//程序中运行状态信息
	int startTime=INF;//真实发车时间
	int endTime=INF;//真实到达时间
	int state;
	bool isInRoad;
	struct  carInChannelState{
		int currentRoadId;		//车辆当前所在的道路id
		int nextRoadId;			//下一条路的道路id
		int nextCross;			//下一个要出的路口
		int direction;			//转向状态
		int roadDir;			//道路方向
		int pathIndex = 0;		//nextRoadId在列表中的位置
		int currentChannel;     //所在车道
		//update更新
		int currentSpeed;		//当前最大速度
		int pos;
	} currentInf;
	bool operator<(const Car &b){
		return this->sortFlag > b.sortFlag;
	}

};

class Road
{
public:
	//基本信息
	int id;
	int index;
	int length;
	int speed;
	int channel;
	int isDuplex;
	int truefrom;
	int trueto;
	int capacity=0;
	int maxNum;
	int getfrom(){//将.txt给的Road的from转为
		return g_crossDatas.getIndex(this->truefrom);
	}
	int  getto(){
		return g_crossDatas.getIndex(this->trueto);
	}
	//临时信息
	unsigned short roadStatus[2][MaxTime];//道路时间状态		//区分双向道路，0:from 1:to
	bool isFullTime[2][MaxTime];
	vector<list<int>> onRoadCars[2];	//区分双向道路，vector各个车道 list车的id,及位置//0:from 1:to
	list<int> carSequeue[2];
	list<pair<int,int>> carGeneralGarage[2];//普通车库 <id time>
	list<pair<int,int>> carPriorityGarage[2];//优先车车库<id time>
	void createGarage();
	void createSequeue(const int &dir);
	void runCarInInitList(const int & dir,const int & time,const bool &priority);
};
//CarDatas-----------------------
class CarDatas{
public:
	CarDatas(){};
	void open(string &carPath);
	void readAnswer(const string &presetAnswerPath,const string &answerPath);

	~CarDatas()	{};
	Car& operator[](const int &index){ return m_car[index]; };
	Car& findById(const int &id){
		if (m_idToIndex.count(id) != 0){
			return m_car[m_idToIndex[id]];
		}
		cout << "CarDatas findById Error:"<< id << endl;
		getchar();
	};
	Car& orderCarsList(int &i){
		return findById(m_sort_by_speed[i].id);
	};
	const int size(){ return m_car.size(); };
	
	void push_back(CarDatas &cars,const int &carId){
        m_carsId.push_back(carId);
        m_car.push_back(cars.findById(carId));
        m_idToIndex[carId] = m_totalNum;
        m_totalNum++;
    }
    void clear(){
        m_totalNum = 0;
        m_carsId.clear();
        m_car.clear();
        m_idToIndex.clear();
    }
	int m_totalNum = 0;
	vector<int> m_carsId;
	vector<Car> m_car;
	vector<Car> m_sort_by_speed;//里面的car不能使用，只做排序用

	unordered_map<int, int> m_idToIndex;

};

//RoadDatas-----------------------
class RoadDatas{
public:
	RoadDatas(){};
	void open(string &roadPath);

	~RoadDatas(){};
	Road& operator[](const int &index){ return m_road[index]; };
	Road& findByFromTo(const int &from,const int &to);
	Road& findById(const int &id){
		if (m_idToIndex.count(id) != 0){
			return m_road[m_idToIndex[id]];
		}
		cout << "RoadDatas findById Error"<< id<< endl;
		getchar();
	};
	const int size(){ return m_road.size(); };
	vector<Road> m_road;
	int firstid = 0;
private:
	unordered_map<int, int> m_FT_2_id;
	unordered_map<int, int> m_idToIndex;
};

extern CarDatas g_carDatas;
extern RoadDatas g_roadDatas;

#endif

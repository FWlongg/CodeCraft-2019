//2019-4-14
//Author F.J.S.
#include "pantiqi.h"
//判题器使用的全局变量，为什么不用m_carDatas，m_roadDatas，m_crossDatas，懒啊，每次在修改函数和定义之间徘徊的我决定还是简单点吧
CarDatas g_p_carDatas;
RoadDatas g_p_roadDatas;
CrossDatas g_p_crossDatas;
//车辆状态计数器，注意别以为没写num就不是num，大写的才是状态，小写是对应的计数器
int g_no_road_waiting_state = 0;//没上路等待
int g_in_road_end_state = 0;//当前时间片终止
int g_in_road_waiting_state = 0;//当前时间片等待
int g_no_road_arrived_state = 0;//到达终点
int g_in_road_start_state = 0;//时间片的起始状态
int g_in_road_outroad_state = 0;//在路上等待转弯

//判决器
int LOCK_FLAG  =  0;//死锁标志
int G_TIME_INDEX = 0;//调度时间
void PanTiQi::updateRoad(){
	g_crossDatas = g_p_crossDatas;
	g_roadDatas = g_p_roadDatas;
	g_carDatas = g_p_carDatas;
}
void PanTiQi::clear(){
	g_no_road_waiting_state = 0;
	g_in_road_end_state = 0;
	g_in_road_waiting_state = 0;
	g_no_road_arrived_state = 0;
	g_in_road_start_state = 0;
	g_in_road_outroad_state = 0;
	LOCK_FLAG = 0;
	G_TIME_INDEX = 0;
	m_carDatas.clear();
}
void addState(const int &state){
    switch (state)
        {
            case NO_ROAD_WAITING_STATE:
                g_no_road_waiting_state++;
                break;
            case IN_ROAD_START_STATE:
                g_in_road_start_state++;
                break;
            case IN_ROAD_WAITING_STATE:
                g_in_road_waiting_state++;
                break;
            case IN_ROAD_OUTROAD_STATE:
                g_in_road_outroad_state++;
                break;
            case IN_ROAD_END_STATE:
                g_in_road_end_state++;
                break;
            case NO_ROAD_ARRIVED_STATE:
                g_no_road_arrived_state++;
                break;
            default:
                break;
        }
}
void subState(const int &state){
    switch (state)
        {
            case NO_ROAD_WAITING_STATE:
                g_no_road_waiting_state--;
                break;
            case IN_ROAD_START_STATE:
                g_in_road_start_state--;
                break;
            case IN_ROAD_WAITING_STATE:
                g_in_road_waiting_state--;
                break;
            case IN_ROAD_OUTROAD_STATE:
                g_in_road_outroad_state--;
                break;
            case IN_ROAD_END_STATE:
                g_in_road_end_state--;
                break;
            case NO_ROAD_ARRIVED_STATE:
                g_no_road_arrived_state--;
                break;
            default:
                break;
        }
}
void updateCarState(Car &car,const int &state){
    addState(state);
    subState(car.state);
    car.state = state;
};
bool orderCarInSequeue(const pair<int,int> &a,const pair<int,int> &b){
	if(a.second<b.second){
		return true;
	}else if(a.second==b.second){
		if(a.first<b.first){
			return true;
		}
	}
	return false;
}
bool AllCarisState(CarDatas &carDatas, const int &state){
	for (int i = 0; i < carDatas.size(); i++){
		if (state != carDatas[i].state){
			return false;
		}
	}
	return true;
}
bool AllCarExitState(CarDatas &carDatas, const int &state){
	for (int i = 0; i < carDatas.size(); i++){
		if ((state & carDatas[i].state)>0){
			return true;
		}
	}
	return false;
}
bool carIsState(Car &car, const int &state){
	if ((state & car.state)>0){
		return true;
	}
	return false;
}
int initState(){
	for (int i = 0; i < g_p_carDatas.size(); i++){
		if (g_p_carDatas[i].isInRoad&&g_p_carDatas[i].state == IN_ROAD_END_STATE){
            updateCarState(g_p_carDatas[i],IN_ROAD_START_STATE);
		}
	}
	return 0;
}
/* 道路状况显示 */
void showStatus(int &time, RoadDatas &roads,const int &mode){
    if(0==mode){
        cout << time <<"\t" <<endl;
        int start = time-10>0?time-10:0;
        int end = time+ 60;
        for (int i = 0; i < roads.size(); i++){
            cout << roads[i].id << "-\t";
            for (int j = start; j <end; j++){
                if (roads[i].roadStatus[0][j] == 0)
                cout << ".";
                else
                cout << roads[i].roadStatus[0][j] << ".";
            }
            cout << "\t" << roads[i].capacity;
            cout << endl;
            cout << roads[i].id << " \t";
            for (int j = start; j < end; j++){
                if (roads[i].roadStatus[1][j] == 0)
                cout << ".";
                else
                cout << roads[i].roadStatus[1][j] << ".";
            }
            cout << "\t" << roads[i].capacity;
            cout << endl;
        }
        cout << endl;
    }
    else if(1==mode){
        cout << time <<"\t" <<endl;
		string exitString("++");
		string emptyString("  ");
		string nullString("--");
		string displayString;
        for (int i = 0; i < roads.size(); i++){
            int total = roads[i].length *roads[i].channel;
            int percent = 0;
            percent = roads[i].roadStatus[0][time]*100/total;
            //正向
			cout << roads[i].id << "-\t";
            int start = 0;
            int end = percent;
			displayString = emptyString;
             cout <<total<<"\t";
            for (int j = start; j <end; j++){              
                cout << exitString;
            }
            for (int j = end; j <100; j++){              
                cout << displayString;
            }
            cout << "\%" << percent;
            cout<<"\t";
			for (int j = 0; j < roads[i].roadStatus[0][time]; j++){
                cout << "-";
            }
			cout << endl;
			//反向
			if(roads[i].isDuplex==0){
				continue;
				displayString = nullString;
			}
            cout << roads[i].id << " \t";
            percent = roads[i].roadStatus[1][time]*100/total;
            end = percent;
            cout <<total<<"\t";
			
			
            for (int j = start; j <end; j++){              
                cout << exitString;
            }
            for (int j = end; j <100; j++){              
                cout << displayString;
            }
            cout << "\%" << percent;
			cout<<"\t";
			for (int j = 0; j < roads[i].roadStatus[1][time]; j++){
                cout << "-";
            }
            cout << endl;
        }
    }
    
}
//这个是打印各个时间片的状态信息log的，修改245行的路径是存放目录，因为后面不用了所以打印输出并没有更新，也不知道有没有问题
void showAllCarCurrentState(const int &timeIndex){
	std::cout << "Current Time: " << timeIndex << endl;

	int no_road_waiting_state = 0;
	int in_road_end_state = 0;
	int in_road_waiting_state = 0;
	int no_road_arrived_state = 0;
	int in_road_start_state = 0;
	int in_road_outroad_state = 0;
	for (int i = 0; i < g_p_carDatas.size(); i++){
		const int IN_ROAD_OUTROAD_STATE = 8;
		if (NO_ROAD_WAITING_STATE == g_p_carDatas[i].state){
			no_road_waiting_state++;
		}
		if (IN_ROAD_START_STATE == g_p_carDatas[i].state){
			in_road_start_state++;
		}
		if (IN_ROAD_WAITING_STATE == g_p_carDatas[i].state){
			in_road_waiting_state++;
		}
		if (IN_ROAD_OUTROAD_STATE == g_p_carDatas[i].state){
			in_road_outroad_state++;
		}
		if (IN_ROAD_END_STATE == g_p_carDatas[i].state){
			in_road_end_state++;
		}
		
		if (NO_ROAD_ARRIVED_STATE == g_p_carDatas[i].state){
			no_road_arrived_state++;
		}

	}
	ofstream outfile;
	string path = string("/home/out/carStatus") + to_string(timeIndex) +string(".txt");
	outfile.open(path);
	if (!outfile.is_open())
	{
		std::cout << "can not open carPath file:" << path << endl;
		exit(0);
	}
	//carDatas
	outfile<<"Current Time: " << timeIndex << endl;
	outfile<<" (1.index 2.id 3.speed 4.currentRoadId 5.roadDir 6.currentChannel 7.pos 8.dir 9.ntCro 10.nRId 11.pIdex)"<<endl;
	outfile<<" 1index";
	outfile<<" \t2id  ";
	outfile<<" \t3sp";
	outfile<<" \t4crID";
	outfile<<" \t5rDir";
	outfile<<" \t6cChanl";
	outfile<<" \t7pos";

	outfile<<" \t8dir";
	outfile<<" \t9ntCro";
	outfile<<" \t10nRId";
	outfile<<" \t11pIdex";
	outfile<<" \t12state";
	outfile<<endl;
	for(int i=0;i<g_p_carDatas.size();i++){
		if(g_p_carDatas[i].isInRoad){
			outfile<<" "<<g_p_carDatas[i].index;
			outfile<<"     \t"<<g_p_carDatas[i].id;
			outfile<<" \t"<<g_p_carDatas[i].speed;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.currentRoadId;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.roadDir;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.currentChannel;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.pos;

			outfile<<" \t"<<g_p_carDatas[i].currentInf.direction;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.nextCross;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.nextRoadId;
			outfile<<" \t"<<g_p_carDatas[i].currentInf.pathIndex;
			outfile<<" \t"<<g_p_carDatas[i].state;

			outfile<<endl;
			outfile<<endl;
		}
	}
	outfile.close();
	//roadDatas
	path = string("/home/out/roadStatus") + to_string(timeIndex) +string(".txt");
	outfile.open(path);
	outfile << "NO_ROAD_WAITING_STATE: " << no_road_waiting_state << endl;
	outfile << "IN_ROAD_START_STATE: " << in_road_start_state << endl;
	outfile << "IN_ROAD_WAITING_STATE: " << in_road_waiting_state << endl;
	outfile << "IN_ROAD_OUTROAD_STATE: " << in_road_outroad_state << endl;
	outfile << "IN_ROAD_END_STATE: " << in_road_end_state << endl;
	outfile << "NO_ROAD_ARRIVED_STATE: " << no_road_arrived_state << endl;

	outfile<<" (1.index 2.id 3.speed 4.getfrom 5.getto 6.length 7.channel 8 isD)"<<endl;
	for(int i=0;i<g_p_roadDatas.size();i++){

		outfile<<"Road "<<g_p_roadDatas[i].index;
		outfile<<" \t"<<g_p_roadDatas[i].id;
		outfile<<" \t"<<g_p_roadDatas[i].speed;
		outfile<<" \t"<<g_p_roadDatas[i].getfrom();
		outfile<<" \t"<<g_p_roadDatas[i].getto();
		outfile<<" \t"<<g_p_roadDatas[i].length;
		outfile<<" \t"<<g_p_roadDatas[i].channel;
		outfile<<" \t"<<g_p_roadDatas[i].isDuplex<<endl;
		for(int channel=0;channel<g_p_roadDatas[i].channel;channel++){
			outfile<< "dir: 0"<<" channel: "<< channel<<": ";
			for(list<int>::iterator it = g_p_roadDatas[i].onRoadCars[0][channel].begin();it!=g_p_roadDatas[i].onRoadCars[0][channel].end();it++){
				Car car = g_p_carDatas.findById(*it);
				//outfile<<"car: "<<g_p_carDatas[i].index;
				outfile<<" (";
					if(1==car.priority){
						outfile<<"vip ";
					}
					else{
						outfile<<"nor ";
					}
					if(car.currentInf.direction==GO_STRAIGHT){
						outfile<<",D";
					}
					else if(car.currentInf.direction==TURN_RIGHT){
						outfile<<",R";
					}
					else if(car.currentInf.direction==TURN_LEFT){
						outfile<<",L";
					}
					outfile<<car.id;
					
					//outfile<<" \t"<<car.currentInf.currentRoadId;
					//outfile<<" \t"<<car.currentInf.roadDir;
					//outfile<<" \t"<<car.currentInf.currentChannel;
					outfile<<", "<<car.currentInf.pos;
					if(car.state==IN_ROAD_WAITING_STATE){
						outfile<<", WT";
					}
					else if(car.state==IN_ROAD_OUTROAD_STATE){
						outfile<<", OT";
					}
					else if(car.state==IN_ROAD_END_STATE){
						outfile<<", ED";
					}
					
					outfile<<", "<<car.speed;
					//outfile<<", nr"<<car.currentInf.nextRoadId;
					
					outfile<<")";
				//outfile<<" \t"<<car.currentInf.direction;
				//outfile<<" \t"<<car.currentInf.nextCross;
				//outfile<<" \t"<<car.currentInf.pathIndex;

				
			}
			outfile<<endl;
		}
		if(1==g_p_roadDatas[i].isDuplex){
			for(int channel=0;channel<g_p_roadDatas[i].channel;channel++){
				outfile<< "dir: 1"<<" channel: "<< channel<<": ";
				for(list<int>::iterator it = g_p_roadDatas[i].onRoadCars[1][channel].begin();it!=g_p_roadDatas[i].onRoadCars[1][channel].end();it++){
					Car car = g_p_carDatas.findById(*it);
					//outfile<<"car: "<<g_p_carDatas[i].index;
					outfile<<" (";
					if(1==car.priority){
						outfile<<"vip ";
					}
					else{
						outfile<<"nor ";
					}
					if(car.currentInf.direction==GO_STRAIGHT){
						outfile<<",D";
					}
					else if(car.currentInf.direction==TURN_RIGHT){
						outfile<<",R";
					}
					else if(car.currentInf.direction==TURN_LEFT){
						outfile<<",L";
					}
					outfile<<car.id;
					
					//outfile<<" \t"<<car.currentInf.currentRoadId;
					//outfile<<" \t"<<car.currentInf.roadDir;
					//outfile<<" \t"<<car.currentInf.currentChannel;
					outfile<<", "<<car.currentInf.pos;
					if(car.state==IN_ROAD_WAITING_STATE){
						outfile<<", WT";
					}
					else if(car.state==IN_ROAD_OUTROAD_STATE){
						outfile<<", OT";
					}
					else if(car.state==IN_ROAD_END_STATE){
						outfile<<", ED";
					}
					
					outfile<<", "<<car.speed;
					//outfile<<", nr"<<car.currentInf.nextRoadId;
					
					outfile<<")";

					//outfile<<" \t"<<car.currentInf.nextCross;
					//outfile<<" \t"<<car.currentInf.nextRoadId;
					//outfile<<" \t"<<car.currentInf.pathIndex;

				}
				outfile<<endl;
			}
			
		}
		outfile<<endl;
	}
	outfile<<endl;
	outfile<<endl;
	outfile.close();
	std::cout << "NO_ROAD_WAITING_STATE: " << no_road_waiting_state << endl;
	std::cout << "IN_ROAD_START_STATE: " << in_road_start_state << endl;
	std::cout << "IN_ROAD_WAITING_STATE: " << in_road_waiting_state << endl;
	std::cout << "IN_ROAD_OUTROAD_STATE: " << in_road_outroad_state << endl;
	std::cout << "IN_ROAD_END_STATE: " << in_road_end_state << endl;
	std::cout << "NO_ROAD_ARRIVED_STATE: " << no_road_arrived_state << endl;
}
//获取每辆车转弯信息，TURN_LEFT？GO_STRAIGHT？TURN_RIGHT？
int getCarTurnDirection(const Road &fromRoad,const Road &toRoad,const Cross &cross){
	int fromIndex = -1;
	int toIndex=-1;
	for (int i = 0; i < 4; i++){
		if (fromRoad.id == cross.roads[i]){
			fromIndex = i;
		}
		if (toRoad.id == cross.roads[i]){
			toIndex = i;
		}
	}
	if (-1 == fromIndex || -1 == toIndex){
		std::cout << "getCarTurnDirection fromIndex or toIndex wrong " << endl;
		exit(1);
	}
	int temp = (toIndex - fromIndex)+4;
	if (temp%4==1){
		return TURN_LEFT;
	}
	else if (temp%4==2){
		return GO_STRAIGHT;
	}
	else{
		return  TURN_RIGHT;
	}
	std::cout << "getCarTurnDirection Wrong"<<fromRoad.id<<" "<< toRoad.id<<" "<< cross.id<<endl;
	return 0;
}
//车辆转向之后更新相应道路的信息
int updateCarOutRoadStatus(Car &currentCar, const int &roadDir, const int &toChannel, const int &currentCross){
	//roadDir 始终未车辆当前的道路方向
	//只更新 car.currentInf currentRoadId roadDir nextRoadId nextCross  currentChannel direction pathIndex
	if(currentCar.path_roadId.size()<2){
		int firstRoadId = currentCar.path_roadId[0];
		int firstCross = (g_p_roadDatas.findById(firstRoadId).getfrom() == currentCar.getfrom()) ? g_p_roadDatas.findById(firstRoadId).getto() : g_p_roadDatas.findById(firstRoadId).getfrom();
		int carTurnDir = GO_STRAIGHT;
		int nextCross = (g_p_roadDatas.findById(firstRoadId).getfrom() == firstCross) ? g_p_roadDatas.findById(firstRoadId).getto() : g_p_roadDatas.findById(firstRoadId).getfrom();
		g_p_carDatas.findById(currentCar.id).currentInf.currentRoadId = firstRoadId;
		g_p_carDatas.findById(currentCar.id).currentInf.roadDir =  roadDir;
		g_p_carDatas.findById(currentCar.id).currentInf.nextRoadId =  NO_ROAD_ARRIVED_STATE;
		g_p_carDatas.findById(currentCar.id).currentInf.nextCross =  nextCross;
		g_p_carDatas.findById(currentCar.id).currentInf.currentChannel = toChannel;
		g_p_carDatas.findById(currentCar.id).currentInf.direction = carTurnDir;
		g_p_carDatas.findById(currentCar.id).currentInf.pathIndex = 0;
		return 0;
	}
	if(currentCar.currentInf.pathIndex+2>=currentCar.path_roadId.size()&&currentCar.isInRoad){
		int currentRoadId = currentCar.currentInf.currentRoadId;
		int turnToRoadId = currentCar.currentInf.nextRoadId;
		int lastCross =currentCar.currentInf.nextCross;
		int nextCross = (g_p_roadDatas.findById(turnToRoadId).getfrom() == lastCross) ? g_p_roadDatas.findById(turnToRoadId).getto() : g_p_roadDatas.findById(turnToRoadId).getfrom();
		int carTurnDir = GO_STRAIGHT;
		g_p_carDatas.findById(currentCar.id).currentInf.currentRoadId = turnToRoadId;
		g_p_carDatas.findById(currentCar.id).currentInf.roadDir =  roadDir;
		g_p_carDatas.findById(currentCar.id).currentInf.nextRoadId =  NO_ROAD_ARRIVED_STATE;
		g_p_carDatas.findById(currentCar.id).currentInf.nextCross =  nextCross;
		g_p_carDatas.findById(currentCar.id).currentInf.currentChannel = toChannel;
		g_p_carDatas.findById(currentCar.id).currentInf.direction = carTurnDir;
		g_p_carDatas.findById(currentCar.id).currentInf.pathIndex++;
		return 0;
	}
	if(!currentCar.isInRoad&&currentCar.state!=NO_ROAD_ARRIVED_STATE){	//未上路更新
		int firstRoadId = currentCar.path_roadId[0];
		int secondRoadId = currentCar.path_roadId[1];
		int from = currentCar.getfrom();
		int firstCross = (g_p_roadDatas.findById(firstRoadId).getfrom() == from) ? g_p_roadDatas.findById(firstRoadId).getto() : g_p_roadDatas.findById(firstRoadId).getfrom();
		int carTurnDir = getCarTurnDirection(g_p_roadDatas.findById(firstRoadId),g_p_roadDatas.findById(secondRoadId),g_p_crossDatas.findById(firstCross));
		g_p_carDatas.findById(currentCar.id).currentInf.currentRoadId = firstRoadId;
		g_p_carDatas.findById(currentCar.id).currentInf.roadDir =  roadDir;
		g_p_carDatas.findById(currentCar.id).currentInf.nextRoadId =  secondRoadId;
		g_p_carDatas.findById(currentCar.id).currentInf.nextCross =  firstCross;
		g_p_carDatas.findById(currentCar.id).currentInf.currentChannel = toChannel;
		g_p_carDatas.findById(currentCar.id).currentInf.direction = carTurnDir;
		g_p_carDatas.findById(currentCar.id).currentInf.pathIndex = 0;
	}else{	//上路更新，我感觉快我快失了智
		int pathIndex = g_p_carDatas.findById(currentCar.id).currentInf.pathIndex;
		int currentRoadId = currentCar.currentInf.currentRoadId;
		int turnToRoadId = currentCar.currentInf.nextRoadId;
		int nextNextRoadId = currentCar.path_roadId[pathIndex+2];
		int lastCross =currentCar.currentInf.nextCross;
		int nextCross = (g_p_roadDatas.findById(turnToRoadId).getfrom() == lastCross) ? g_p_roadDatas.findById(turnToRoadId).getto() : g_p_roadDatas.findById(turnToRoadId).getfrom();
		int carTurnDir = getCarTurnDirection(g_p_roadDatas.findById(turnToRoadId),g_p_roadDatas.findById(nextNextRoadId),g_p_crossDatas.findById(nextCross));
		g_p_carDatas.findById(currentCar.id).currentInf.currentRoadId = turnToRoadId;;
		g_p_carDatas.findById(currentCar.id).currentInf.roadDir =  roadDir;
		g_p_carDatas.findById(currentCar.id).currentInf.nextRoadId = nextNextRoadId;
		g_p_carDatas.findById(currentCar.id).currentInf.nextCross =  nextCross;
		g_p_carDatas.findById(currentCar.id).currentInf.currentChannel = toChannel;
		g_p_carDatas.findById(currentCar.id).currentInf.direction = carTurnDir;
		g_p_carDatas.findById(currentCar.id).currentInf.pathIndex ++;
	}
	return 0;
}
//道路行驶dir为0，1,3，0为road的from对to方向 1为to到from方向
void driveCarInitList(const Road &currentRoad,const int &dir,const int &time,const bool &priority){
	g_p_roadDatas.findById(currentRoad.id).runCarInInitList(dir,time,priority);
}
int driveCarInitList(const int &time,const bool &priority){
	for(int i=0;i<g_p_roadDatas.size();i++){
		g_p_roadDatas[i].runCarInInitList(3,time,priority);
	}
	return 0;
}
//优先车辆的显示
bool roadExistVip(const int &carLev,const int &crossId,const int &currentRoadId,const int &checkRoadId,const int &priority){	
	Road checkRoad = g_p_roadDatas.findById(checkRoadId);
	int direction = (checkRoad.getfrom() == crossId) ? 1 : 0;
	bool print = false;
	if(1==priority){
		if(checkRoad.carSequeue[direction].empty()){
			return false;
		}
		if((g_p_carDatas.findById(checkRoad.carSequeue[direction].front()).priority>=priority)
		&&(g_p_carDatas.findById(checkRoad.carSequeue[direction].front()).currentInf.direction == carLev)){
			if(print)std::cout<<"Conflict Car:"<< g_p_carDatas.findById(checkRoad.carSequeue[direction].front()).id<<endl;
			return true;
		}
	}else{
		Cross cross= g_p_crossDatas.findById(crossId); 
		Road road;
		Car car;
		int dir;
		for(int i=0;i<4;i++){
			if(cross.roads[i]!=-1&&cross.roads[i]!=currentRoadId){
				road  = g_p_roadDatas.findById(cross.roads[i]);
				dir = (road.getfrom() == crossId) ? 1 : 0;
				if(!road.carSequeue[dir].empty()){
					car = g_p_carDatas.findById(road.carSequeue[dir].front());
					if(car.priority==1){
						if(print)std::cout<<"Conflict Car:"<< car.id<<endl;
						return true;
					}
				}
			}
		}
		if(checkRoad.carSequeue[direction].empty()){
			return false;
		}
		if((g_p_carDatas.findById(checkRoad.carSequeue[direction].front()).priority>=priority)
		&&(g_p_carDatas.findById(checkRoad.carSequeue[direction].front()).currentInf.direction == carLev)){
			if(print)std::cout<<"Conflict Car:"<< g_p_carDatas.findById(checkRoad.carSequeue[direction].front()).id<<endl;
			return true;
		}
	}
	
	return false;
}
//冲突检测
bool moveToNextRoadConflict(const Car &car){
	int currentRoadId = car.currentInf.currentRoadId;
	int nextRoadId = car.currentInf.nextRoadId;
	int direction = car.currentInf.direction;
	int curdir = car.currentInf.roadDir;
	int channel = car.currentInf.currentChannel;
	int crossId = car.currentInf.nextCross;
	bool print = false;
	Cross cross= g_p_crossDatas.findById(crossId); 
	if (IN_ROAD_OUTROAD_STATE == car.state/*处于要出路口状态*/){
		if (GO_STRAIGHT == car.currentInf.direction){
			if(1==car.priority){
				return false;
			}
			Cross cross= g_p_crossDatas.findById(crossId); 
			Road road;
			Car frontcar;
			int dir;
			for(int i=0;i<4;i++){
				if(cross.roads[i]!=-1&&cross.roads[i]!=currentRoadId){
					road  = g_p_roadDatas.findById(cross.roads[i]);
					dir = (road.getfrom() == crossId) ? 1 : 0;
					if(!road.carSequeue[dir].empty()){
						frontcar = g_p_carDatas.findById(road.carSequeue[dir].front());
						if(frontcar.priority==1){
							if(print)std::cout<<"Conflict Car:"<< frontcar.id<<endl;
							return true;
						}
					}
					
				}
			}
			return false;
		}
		Road turnToRoad = g_p_roadDatas.findById(nextRoadId);//转向的下一条路
		int nextdir = (crossId == turnToRoad.getfrom()) ? 0 : 1;
		if(1==car.priority){
			if (direction == TURN_LEFT){
				int rightIndex = (4 + (cross.roadIndex[currentRoadId] - 1)) % 4;
				if ((-1 != cross.roads[rightIndex] )
				&&( roadExistVip(GO_STRAIGHT, crossId, currentRoadId,cross.roads[rightIndex],1))){
					
					return true;
				}
			}
			else if (direction == TURN_RIGHT){
				int leftIndex = (4 + (cross.roadIndex[currentRoadId] + 1)) % 4;
				if ((-1!=cross.roads[leftIndex] )
				&& (roadExistVip(GO_STRAIGHT, crossId,currentRoadId ,cross.roads[leftIndex],1))){
					return true;
				}
				int frontIndex = (4 + (cross.roadIndex[currentRoadId] + 2)) % 4;
				if ((-1 != cross.roads[frontIndex]) 
				&& (roadExistVip(TURN_LEFT, crossId,currentRoadId, cross.roads[frontIndex],1))){
					return true;
				}
			}
			return false;
		}
		if (direction == TURN_LEFT){
			int rightIndex = (4 + (cross.roadIndex[currentRoadId] - 1)) % 4;
			if ((-1 != cross.roads[rightIndex] )
			&&( roadExistVip(GO_STRAIGHT, crossId, currentRoadId,cross.roads[rightIndex],0))
			){
				return true;
			}
		}
		else if (direction == TURN_RIGHT){
			int leftIndex = (4 + (cross.roadIndex[currentRoadId] + 1)) % 4;
			if ((-1!=cross.roads[leftIndex] )
			&& (roadExistVip(GO_STRAIGHT, crossId, currentRoadId,cross.roads[leftIndex],0))){
				return true;
			}
			int frontIndex = (4 + (cross.roadIndex[currentRoadId] + 2)) % 4;
			if ((-1 != cross.roads[frontIndex]) 
			&& roadExistVip(TURN_LEFT, crossId, currentRoadId,cross.roads[frontIndex],0)){
				return true;
			}
		}
	}
	else{
		std::cout<< "moveToNextRoadConflict Car State is Wrong:"<<car.id<<endl;
		exit(1);
	}
	return false;
}
//转向  终点
bool moveToNextRoad(Car &car){
	int curRoadId = car.currentInf.currentRoadId;
	int toRoadId = car.currentInf.nextRoadId;
	//int direction = car.currentInf.direction;
	//int curdir = car.currentInf.roadDir;
	int crossId = car.currentInf.nextCross;
	int curchanl = car.currentInf.currentChannel;
	int curdir = car.currentInf.roadDir;

	if(NO_ROAD_ARRIVED_STATE == toRoadId){
		g_p_carDatas.findById(car.id).currentInf.pos = 0;
        updateCarState(g_p_carDatas.findById(car.id),NO_ROAD_ARRIVED_STATE);
		//g_p_carDatas.findById(car.id).state = NO_ROAD_ARRIVED_STATE;
		g_p_carDatas.findById(car.id).isInRoad = false;
		g_p_roadDatas.findById(curRoadId).onRoadCars[curdir][curchanl].pop_front();
		g_p_roadDatas.findById(curRoadId).carSequeue[curdir].pop_front();
		
		g_p_carDatas.findById(car.id).endTime = G_TIME_INDEX;
		return  true;
	}

	Road toRoad = g_p_roadDatas.findById(toRoadId); 
	Road curRoad = g_p_roadDatas.findById(curRoadId);
	int todir = (crossId == toRoad.getfrom()) ? 0 : 1;

	for (int tochannelIndex = 0; tochannelIndex < toRoad.channel; tochannelIndex++/*下一条路的所有车道*/){
		int backposition;
		if (toRoad.onRoadCars[todir][tochannelIndex].empty()){
			backposition = 0;
		}
		else{
			backposition = g_p_carDatas.findById(toRoad.onRoadCars[todir][tochannelIndex].back()).currentInf.pos;
		}
		// if (!toRoad.onRoadCars[todir][tochannelIndex].empty() && IN_ROAD_WAITING_STATE == g_p_carDatas.findById(toRoad.onRoadCars[todir][tochannelIndex].back()).state){
		// 	return false;
		// }
		int freeSpace = toRoad.length - backposition;
		if (freeSpace > 0){
			int curSpeed = min(curRoad.speed,car.speed);
			int toSpeed = min(toRoad.speed,car.speed);
			int curRDriveLen = car.currentInf.pos-1;
			int toRDriveLen = toSpeed-curRDriveLen;
			if(toRDriveLen<=0){//可行驶距离为0,停在道路前端，这里有疑问---------------
				toRDriveLen = 0;
                if(1 != g_p_carDatas.findById(car.id).currentInf.pos){
                    LOCK_FLAG =1;
                }
				g_p_carDatas.findById(car.id).currentInf.pos = 1;
                updateCarState(g_p_carDatas.findById(car.id),IN_ROAD_END_STATE);
				//g_p_carDatas.findById(car.id).state = IN_ROAD_END_STATE;
				g_p_roadDatas.findById(curRoad.id).carSequeue[curdir].pop_front();
				return true;
			}
			else if (toRDriveLen< freeSpace){//可进入且没有阻挡
				if(car.id != g_p_roadDatas.findById(curRoad.id).onRoadCars[curdir][curchanl].front()){
					std::cout<< "moveToNextRoad The Car is Wrong:"<<endl<< "car:"<<car.id <<" Road:"<<curRoad.id;
					exit(1);
				}
				LOCK_FLAG =1;
				g_p_carDatas.findById(car.id).currentInf.pos = toRoad.length - toRDriveLen + 1;
                updateCarState(g_p_carDatas.findById(car.id),IN_ROAD_END_STATE);
				//g_p_carDatas.findById(car.id).state = IN_ROAD_END_STATE;
				g_p_roadDatas.findById(toRoad.id).onRoadCars[todir][tochannelIndex].push_back(car.id);
				g_p_roadDatas.findById(curRoad.id).onRoadCars[curdir][curchanl].pop_front();
				g_p_roadDatas.findById(curRoad.id).carSequeue[curdir].pop_front();
				updateCarOutRoadStatus(car, todir,tochannelIndex, crossId);
				return true;
			}
			else if (toRDriveLen>= freeSpace && !toRoad.onRoadCars[todir][tochannelIndex].empty()
			&&carIsState(g_p_carDatas.findById(toRoad.onRoadCars[todir][tochannelIndex].back()),IN_ROAD_END_STATE)){
				if(car.id != g_p_roadDatas.findById(curRoad.id).onRoadCars[curdir][curchanl].front()){
					std::cout<< "moveToNextRoad The Car is Wrong:"<<endl<< "car:"<<car.id <<" Road:"<<curRoad.id;
					exit(1);
				}
				LOCK_FLAG =1;
				g_p_carDatas.findById(car.id).currentInf.pos = backposition + 1;
                updateCarState(g_p_carDatas.findById(car.id),IN_ROAD_END_STATE);
				//g_p_carDatas.findById(car.id).state = IN_ROAD_END_STATE;
				g_p_roadDatas.findById(toRoad.id).onRoadCars[todir][tochannelIndex].push_back(car.id);
				g_p_roadDatas.findById(curRoad.id).onRoadCars[curdir][curchanl].pop_front();
				g_p_roadDatas.findById(curRoad.id).carSequeue[curdir].pop_front();
				updateCarOutRoadStatus(car, todir,tochannelIndex, crossId);
				return true;
			}
			else if (toRDriveLen>= freeSpace && !toRoad.onRoadCars[todir][tochannelIndex].empty()
			&&!carIsState(g_p_carDatas.findById(toRoad.onRoadCars[todir][tochannelIndex].back())
			,IN_ROAD_OUTROAD_STATE+IN_ROAD_WAITING_STATE)){
				
				return false;
			}

		}
	}
	//下一道路的全满
	g_p_roadDatas.findById(curRoad.id).carSequeue[curdir].pop_front();
	g_p_carDatas.findById(car.id).currentInf.pos = 1;
    updateCarState(g_p_carDatas.findById(car.id),IN_ROAD_END_STATE);
	//g_p_carDatas.findById(car.id).state = IN_ROAD_END_STATE;
	return true;

}

//<id,time>
//当前道路dir（取值0,1）方向车辆行驶
int driveJustCurrentRoad(Road &currentRoad,int &channel,const int &dir){
	list<int> carsInRoadChannel = g_p_roadDatas.findById(currentRoad.id).onRoadCars[dir][channel];
	if (carsInRoadChannel.size() == 0){
		//std::cout << "WHY ARE YOU SO STUPID!" << endl;
		return -1;
	}
	if(1!=currentRoad.isDuplex && 1==dir){
		std::cout << "WHY ARE YOU SO STUPID! THE DIRECTION IS WRONG" << endl;
	}
	list<int>::iterator itcar;
	int lastCarPos = 0;
	int lastCarState = IN_ROAD_END_STATE;
	for (itcar = carsInRoadChannel.begin(); itcar != carsInRoadChannel.end(); itcar++){
		Car carTemp = g_p_carDatas.findById(*itcar);
		if (!carTemp.isInRoad){
			std::cout << "ARE YOU SHI LE ZHI?" << endl;
			return -1;
		}
		if (IN_ROAD_END_STATE == carTemp.state){
			lastCarPos = carTemp.currentInf.pos;
			lastCarState = IN_ROAD_END_STATE;
			continue;
		}
		int maxSpeed = min(currentRoad.speed, carTemp.speed);
		int nextPos = carTemp.currentInf.pos - maxSpeed;
		if (nextPos>lastCarPos)/*W没有阻挡*/{
			if(g_p_carDatas.findById(*itcar).currentInf.pos !=nextPos){
				LOCK_FLAG = 1;
			}
			g_p_carDatas.findById(*itcar).currentInf.pos = nextPos;
            updateCarState(g_p_carDatas.findById(*itcar),IN_ROAD_END_STATE);
			//g_p_carDatas.findById(*itcar).state = IN_ROAD_END_STATE;
            
			lastCarPos = nextPos;
			lastCarState = IN_ROAD_END_STATE;
		}
		else if(IN_ROAD_END_STATE==lastCarState){
			if (nextPos<=0&&0==lastCarPos)/*出路口*/{
                updateCarState(g_p_carDatas.findById(*itcar),IN_ROAD_OUTROAD_STATE);
				//g_p_carDatas.findById(*itcar).state = IN_ROAD_OUTROAD_STATE;
				lastCarState = IN_ROAD_OUTROAD_STATE;
				lastCarPos = carTemp.currentInf.pos;
			}else if (nextPos <= lastCarPos)/*END有阻挡*/{
				if(g_p_carDatas.findById(*itcar).currentInf.pos != lastCarPos+1){
					LOCK_FLAG =1;
				}
				g_p_carDatas.findById(*itcar).currentInf.pos = lastCarPos+1;
                updateCarState(g_p_carDatas.findById(*itcar),IN_ROAD_END_STATE);
				//g_p_carDatas.findById(*itcar).state = IN_ROAD_END_STATE;
				lastCarPos = lastCarPos+1;
				lastCarState = IN_ROAD_END_STATE;
				
			}
		}
		else if (IN_ROAD_WAITING_STATE == lastCarState|| IN_ROAD_OUTROAD_STATE==lastCarState)/*WAIT有阻挡*/{
			lastCarPos = carTemp.currentInf.pos;
			lastCarState = IN_ROAD_WAITING_STATE;
            updateCarState(g_p_carDatas.findById(*itcar),IN_ROAD_WAITING_STATE);
			//g_p_carDatas.findById(*itcar).state = IN_ROAD_WAITING_STATE;
		}
	}
	return 0;
}
void driveJustCurrentRoad(){
	for (int roadIndex = 0; roadIndex < g_p_roadDatas.size(); roadIndex++ /*所有道路*/) {
			int flag = 0;
			for (int channelIndex = 0; channelIndex < g_p_roadDatas[roadIndex].channel; channelIndex++){
				driveJustCurrentRoad(g_p_roadDatas[roadIndex],channelIndex ,0);/* 对所有来自from->to方向车道进行调整 */
				if (1 == g_p_roadDatas[roadIndex].isDuplex){
					driveJustCurrentRoad(g_p_roadDatas[roadIndex],channelIndex ,1);/* 对所有来自to->from方向车道进行调整 */
				}
			}
		}
	return;
}
//这个放到cross应该更舒服，但我直接放到road里面了，省心省力
void createCarGarage(){
	Road roadTemp;
	for(int i=0;i<g_p_roadDatas.size();i++){
		g_p_roadDatas[i].createGarage();//两条道路
	}
}
//检测所有车道是否
bool allChannelIsEnd(Road *road,const int &dir,vector<list<int>::iterator> &v_iterator,const int &channel){
	for(int i=0;i<channel;i++){
		if(v_iterator[i] != road->onRoadCars[dir][i].end()){
			return false;
		}
	}
	return true;
}
//创建优先队列
void Road::createSequeue(const int &dir){
	int channel = this->channel;
	vector<list<int>::iterator> v_iterator;
	for(int i=0;i<channel;i++){
		v_iterator.push_back(this->onRoadCars[dir][i].begin());
	}
	this->carSequeue[dir].clear();
	while(!allChannelIsEnd(this,dir,v_iterator,channel)){
		bool exitPriority = false;
		for(int i=0;i<channel;i++){
			if(v_iterator[i]!=this->onRoadCars[dir][i].end()){
				int carId = *v_iterator[i];
				if(1==g_p_carDatas.findById(carId).priority && IN_ROAD_OUTROAD_STATE == g_p_carDatas.findById(carId).state){
					this->carSequeue[dir].push_back(carId);
					v_iterator[i]++;
					exitPriority = true;
				}
			}
		}
		if(!exitPriority){
			int minPos = INF;
			int minIndex = INF;
			for(int i=0;i<channel;i++){
				if(v_iterator[i]!=this->onRoadCars[dir][i].end()){
					int carId = *v_iterator[i];
					if(g_p_carDatas.findById(*v_iterator[i]).currentInf.pos<minPos && IN_ROAD_OUTROAD_STATE == g_p_carDatas.findById(carId).state){
						minPos = g_p_carDatas.findById(carId).currentInf.pos;
						minIndex = i;
					}
				}
			}
			if(INF ==minIndex){//既没有优先也没有普通
				break;
			}
			this->carSequeue[dir].push_back(g_p_carDatas.findById(*v_iterator[minIndex]).id);
			v_iterator[minIndex]++;
		}
	}
}

void createCarSequeue(){
	for(int i=0;i<g_p_roadDatas.size();i++){
		g_p_roadDatas[i].createSequeue(0);
		if(1 == g_p_roadDatas[i].isDuplex){
			g_p_roadDatas[i].createSequeue(1);
		}
	}
}


bool driveCarInWaitState(const int &timeIndex,Result &result){
	while (0!=g_in_road_waiting_state ||0!=g_in_road_outroad_state/*AllCarExitState(IN_ROAD_WAITING_STATE + IN_ROAD_OUTROAD_STATE)*//* all car in road run into final state */){
		/* driveAllWaitCar() */
        //cout<<"driveCarInWaitState again"<<endl;
		int flag = 0;
		for (int crossIndex = 0; crossIndex < g_p_crossDatas.size(); crossIndex++/*foreach(crosses)*/){
			//路口所有道路,升序
			for (int roadInCrossIndex = 0; roadInCrossIndex < g_p_crossDatas[crossIndex].orderConnectRoads.size(); roadInCrossIndex++/*foreach(roads)*/){
				int roadId = g_p_crossDatas[crossIndex].orderConnectRoads[roadInCrossIndex];
				//道路的各个车道getCarFromRoad(road, dir);
				int dir = (g_p_crossDatas[crossIndex].id == g_p_roadDatas.findById(roadId).getfrom()) ? 1 : 0;//过路口方向，注意1和0
				while(!g_p_roadDatas.findById(roadId).carSequeue[dir].empty()){
					Car car= g_p_carDatas.findById(g_p_roadDatas.findById(roadId).carSequeue[dir].front());
					int channel = car.currentInf.currentChannel;
					if(moveToNextRoadConflict(car)){
						break;
					}
					if (moveToNextRoad(car)){
						flag = 1;
						driveJustCurrentRoad(g_p_roadDatas.findById(roadId),channel,dir);
						g_p_roadDatas.findById(roadId).createSequeue(dir);//这里是当初判题器文档出现问题的地方，如果是picture1的话是这样，如果是picuture2的话是driveCarInitList(timeIndex,true);
						driveCarInitList(g_p_roadDatas.findById(roadId),dir,timeIndex,true);
					}else{
						break;
					}
				}
				
					/* driveAllCarJustOnRoadToEndState该处理内的算法与性能自行考虑 */

				//runCar();
			}

		}
		if(0 == flag){
            showAllCarCurrentState(timeIndex);//道路车辆结束

            cout<< "EASY DEAD LOCK"<<endl;
			for(int i=0;i<g_p_carDatas.size();i++){
				if(carIsState(g_p_carDatas[i],IN_ROAD_OUTROAD_STATE+IN_ROAD_WAITING_STATE)){
					std::cout<<" Car:"<<g_p_carDatas[i].id;
				}
			}
			result.isFail =true;
			return false;
		}
		//showAllCarCurrentState(timeIndex);//道路车辆结束
	}
	return true;
}
bool isFinish(){
	if (AllCarisState(g_p_carDatas, NO_ROAD_ARRIVED_STATE)){
		return true;
	}
	return false;
}
//发车，这个可能是最烂的地方了，因为我把未上路的车直接放对应的起始路上，但又分开类两个名称carPriorityGarage及carGeneralGarage，结果一模一样的代码写了两遍
void Road::runCarInInitList(const int &direction,const int & time,const bool &priority){
	int crossId;
	int carTempId;
	Car carTemp;
	int dir;
	Car roadBackCar;//当前道路的上一辆车
	//int NextRoadid;//车辆下一个道路ID
	Road NextRoad;//车辆下一个道路
	//优先车辆上路
	//from,to->0  to,form->1
	for(dir=0;dir<2;dir++){
		if(0==this->isDuplex && 1==dir){
			break;
		}
		if(direction!=3 && dir != direction){
			break;
		}
		crossId = (dir!=0)?this->getfrom():this->getto();
		list<pair<int,int>>::iterator itpair;
		for(itpair=this->carPriorityGarage[dir].begin();itpair!=this->carPriorityGarage[dir].end();)
		{
			if(itpair->second<=time){
				carTempId = itpair->first;
				carTemp = g_p_carDatas.findById(carTempId);
				if(carTemp.path_roadId[0]!=this->id||carTemp.isInRoad){
					std::cout<< "The Sequeue is wrong"<<endl;
					return;
				}
				//NextRoadid = carTemp.path_roadId[1];
				int speed = min(carTemp.speed , this->speed);
				int channelIndex;
				bool isfull =false,iswait = false;
				for(channelIndex=0;channelIndex<this->channel;channelIndex++){
					if(this->onRoadCars[dir][channelIndex].empty()){
						//更新roaddata
						this->onRoadCars[dir][channelIndex].push_back(carTempId);
						//更新Sequeue
						itpair = this->carPriorityGarage[dir].erase(itpair);
						updateCarOutRoadStatus(g_p_carDatas.findById(carTempId),dir,channelIndex, crossId);
						//更新car
						g_p_carDatas.findById(carTempId).currentInf.pos = this->length - speed+1;
                        updateCarState(g_p_carDatas.findById(carTempId),IN_ROAD_END_STATE);
						//g_p_carDatas.findById(carTempId).state = IN_ROAD_END_STATE;
						g_p_carDatas.findById(carTempId).isInRoad = true;
						
						g_p_carDatas.findById(carTempId).startTime = G_TIME_INDEX;
						LOCK_FLAG =1;
						break;
					}
					else{//车道不为空
						int carId = this->onRoadCars[dir][channelIndex].back();
						roadBackCar = g_p_carDatas.findById(carId);
						int space = this->length - roadBackCar.currentInf.pos;
						if (space > speed && space>0/*道路空间够大加车*/){
							//更新roaddata
							this->onRoadCars[dir][channelIndex].push_back(carTemp.id);
							//更新Sequeue
							itpair = this->carPriorityGarage[dir].erase(itpair);
							updateCarOutRoadStatus(g_p_carDatas.findById(carTempId),dir, channelIndex, crossId);
							//更新cardata
							g_p_carDatas.findById(carTempId).currentInf.pos = this->length - speed+1;
                            updateCarState(g_p_carDatas.findById(carTempId),IN_ROAD_END_STATE);
							//g_p_carDatas.findById(carTempId).state = IN_ROAD_END_STATE;
							g_p_carDatas.findById(carTempId).isInRoad = true;
							
							g_p_carDatas.findById(carTempId).startTime = G_TIME_INDEX;
							LOCK_FLAG =1;
							break;
						}
						else if (space <= speed && space>0 /*道路不为空，速度限制，加车*/){
							if(carIsState(roadBackCar,IN_ROAD_WAITING_STATE+IN_ROAD_OUTROAD_STATE)){
								iswait =true;
								break;//我觉得应该是continue 但是伪码的地方是break;
							}
							//更新roaddata
							this->onRoadCars[dir][channelIndex].push_back(carTemp.id);
							//更新crossdata
							itpair = this->carPriorityGarage[dir].erase(itpair);
							updateCarOutRoadStatus(g_p_carDatas.findById(carTempId), dir, channelIndex, crossId);
							//更新cardata
							g_p_carDatas.findById(carTempId).currentInf.pos = roadBackCar.currentInf.pos+1;
                            updateCarState(g_p_carDatas.findById(carTempId),IN_ROAD_END_STATE);
							//g_p_carDatas.findById(carTempId).state = IN_ROAD_END_STATE;
							g_p_carDatas.findById(carTempId).isInRoad = true;
							
							g_p_carDatas.findById(carTempId).startTime = G_TIME_INDEX;
							LOCK_FLAG =1;
							break;
						}
						else{//车道已满
								isfull = true;
								continue;
						}
					}
				}
				if((channelIndex == this->channel && isfull) || iswait){
					//cout<< this->id<<" is full"<<endl;
					break;
				}
			}else{
				itpair++;
			}
		}
	}
	if(false ==priority){
		for(dir=0;dir<2;dir++){
			if(0==this->isDuplex && 1==dir){
				break;
			}
			if(direction!=3 && dir != direction){
				break;
			}
			crossId = this->getfrom();
			list<pair<int,int>>::iterator itpair;
			for(itpair=this->carGeneralGarage[dir].begin();itpair!=this->carGeneralGarage[dir].end();)
			{
				if(itpair->second<=time){
					carTempId = itpair->first;
					carTemp = g_p_carDatas.findById(carTempId);
					if(carTemp.path_roadId[0]!=this->id||carTemp.isInRoad){
						std::cout<< "The Sequeue is wrong"<<endl;
						return;
					}
					//NextRoadid = carTemp.path_roadId[1];
					int speed = min(carTemp.speed , this->speed);
					int channelIndex;
					bool isfull =false;
					bool iswait =false;
					for(channelIndex=0;channelIndex<this->channel;channelIndex++){
						isfull = false;
						if(this->onRoadCars[dir][channelIndex].empty()){
							//更新roaddata
							this->onRoadCars[dir][channelIndex].push_back(carTempId);
							//更新Sequeue
							itpair = this->carGeneralGarage[dir].erase(itpair);
							//更新car
							updateCarOutRoadStatus(g_p_carDatas.findById(carTempId), dir, channelIndex, crossId);
							g_p_carDatas.findById(carTempId).currentInf.pos = this->length - speed+1;
                            updateCarState(g_p_carDatas.findById(carTempId),IN_ROAD_END_STATE);
							//g_p_carDatas.findById(carTempId).state = IN_ROAD_END_STATE;
							g_p_carDatas.findById(carTempId).isInRoad = true;
							LOCK_FLAG =1;
							break;
						}
						else{//车道不为空
							int carId = this->onRoadCars[dir][channelIndex].back();
							roadBackCar = g_p_carDatas.findById(carId);
							int space = this->length - roadBackCar.currentInf.pos;
							if (space > speed && space>0/*道路空间够大加车*/){
								//更新roaddata
								this->onRoadCars[dir][channelIndex].push_back(carTemp.id);
								//更新Sequeue
								itpair = this->carGeneralGarage[dir].erase(itpair);
								//更新cardata
								updateCarOutRoadStatus(g_p_carDatas.findById(carTempId), dir, channelIndex, crossId);
								g_p_carDatas.findById(carTempId).currentInf.pos = this->length - speed+1;
                                updateCarState(g_p_carDatas.findById(carTempId),IN_ROAD_END_STATE);
								//g_p_carDatas.findById(carTempId).state = IN_ROAD_END_STATE;
								g_p_carDatas.findById(carTempId).isInRoad = true;	
								
								g_p_carDatas.findById(carTempId).startTime = G_TIME_INDEX;
								LOCK_FLAG =1;
								break;						
							}
							else if (space <= speed && space>0 /*道路不为空，速度限制，加车*/){
								if(carIsState(roadBackCar,IN_ROAD_WAITING_STATE+IN_ROAD_OUTROAD_STATE)){
									iswait=true;
									break;//我觉得应该是continue 但是伪码的地方是break;
								}
								//更新roaddata
								this->onRoadCars[dir][channelIndex].push_back(carTemp.id);
								//更新crossdata
								itpair = this->carGeneralGarage[dir].erase(itpair);
								updateCarOutRoadStatus(g_p_carDatas.findById(carTempId), dir, channelIndex, crossId);
								//更新cardata
								g_p_carDatas.findById(carTempId).currentInf.pos = roadBackCar.currentInf.pos+1;
                                updateCarState(g_p_carDatas.findById(carTempId),IN_ROAD_END_STATE);
								//g_p_carDatas.findById(carTempId).state = IN_ROAD_END_STATE;
								g_p_carDatas.findById(carTempId).isInRoad = true;
								
								g_p_carDatas.findById(carTempId).startTime = G_TIME_INDEX;
								LOCK_FLAG =1;
								break;
							}
							else{//车道已满
								isfull = true;
								continue;
							}
						}
					}
					if((channelIndex == this->channel && isfull)|| iswait){
						//std::cout<< this->id<<" is full"<<endl;
						break;
					}
				}else{
					itpair++;
				}
			}
		}
	}
}
void Road::createGarage(){
	for(int i=0;i<g_p_carDatas.size();i++){
		if(!g_p_carDatas[i].isInRoad){
			int firstRoadId = g_p_carDatas[i].path_roadId[0];
			if(firstRoadId == this->id){
				int from= g_p_carDatas[i].getfrom();
				int dir = (from==this->getfrom())?0:1;
				if(0==g_p_carDatas[i].priority){
					this->carGeneralGarage[dir].push_back(make_pair(g_p_carDatas[i].id,g_p_carDatas[i].planTime));
				}
				else{
					this->carPriorityGarage[dir].push_back(make_pair(g_p_carDatas[i].id,g_p_carDatas[i].planTime));
				}
			}
			
		}
	}
	this->carGeneralGarage[0].sort(orderCarInSequeue);
	this->carGeneralGarage[1].sort(orderCarInSequeue);

	this->carPriorityGarage[0].sort(orderCarInSequeue);
	this->carPriorityGarage[1].sort(orderCarInSequeue);
}
int getResult(Result &r){
	int T = G_TIME_INDEX;
	int Tpri = 0;
	int minPTime = INF;
	int priMaxETime = 0;
	for(int i=0;i<g_p_carDatas.size();i++){
		if(g_p_carDatas[i].planTime<minPTime){
			minPTime = g_p_carDatas[i].planTime;
		}
		if(1==g_p_carDatas[i].priority){
			if(g_p_carDatas[i].endTime > priMaxETime){
				priMaxETime = g_p_carDatas[i].endTime;
			}
		}
	}
    Tpri = (0==priMaxETime)?0:priMaxETime-minPTime;
	r.scheduleT = T+ g_v_a*Tpri;
	r.T_or =T;
	r.T_pri = Tpri;
	return r.scheduleT;
}
//RoadStatus为各个道路的各个方向（取值0,1）各个车道（ 0 1.... channel-1）的车辆数量
void updateRoadStatus(const int &time){
    for(int i=0;i<g_p_roadDatas.size();i++){
        g_p_roadDatas[i].roadStatus[0][time] = 0;
        int num = 0;
        for(int j=0;j< g_p_roadDatas[i].channel;j++){
            num +=g_p_roadDatas[i].onRoadCars[0][j].size();
        }
        g_p_roadDatas[i].roadStatus[0][time] =num;

        if(!g_p_roadDatas[i].onRoadCars[0][g_p_roadDatas[i].channel-1].empty()
            &&g_p_roadDatas[i].length <= g_p_carDatas.findById(g_p_roadDatas[i].onRoadCars[0][g_p_roadDatas[i].channel-1].back()).currentInf.pos){
            Car lastcar = g_p_carDatas.findById(g_p_roadDatas[i].onRoadCars[0][g_p_roadDatas[i].channel-1].back());
            g_p_roadDatas[i].isFullTime[0][time] = true;
        }
        else{
            g_p_roadDatas[i].isFullTime[0][time] = false;
        }

        if(1==g_p_roadDatas[i].isDuplex){
            g_p_roadDatas[i].roadStatus[1][time] = 0;
            int num1 = 0;
            for(int j=0;j< g_p_roadDatas[i].channel;j++){
                num1 +=g_p_roadDatas[i].onRoadCars[1][j].size();
            }
            g_p_roadDatas[i].roadStatus[1][time] =num1;
            if(!g_p_roadDatas[i].onRoadCars[1][g_p_roadDatas[i].channel-1].empty()
                &&g_p_roadDatas[i].length <= g_p_carDatas.findById(g_p_roadDatas[i].onRoadCars[1][g_p_roadDatas[i].channel-1].back()).currentInf.pos){
                Car lastcar = g_p_carDatas.findById(g_p_roadDatas[i].onRoadCars[1][g_p_roadDatas[i].channel-1].back());
                g_p_roadDatas[i].isFullTime[1][time] = true;
            }
            else{            
                g_p_roadDatas[i].isFullTime[1][time] = false;
            }
        }
    }
}

void PanTiQi::panjue(Result &result, const bool &isChangeGloble){
    g_p_crossDatas = m_crossDatas;
	g_p_roadDatas = m_roadDatas;
	g_p_carDatas = m_carDatas;
	//车辆状态初始化
	std::cout << "car" << endl;
	for (int i = 0; i < g_p_carDatas.size(); i++){
        //updateCarState(g_p_carDatas[i],NO_ROAD_WAITING_STATE);
		g_p_carDatas[i].state = NO_ROAD_WAITING_STATE;
        addState(NO_ROAD_WAITING_STATE);
		g_p_carDatas[i].isInRoad = false;
	}
	//道路初始化
	std::cout << "road" << endl;
	for (int i = 0; i < g_p_roadDatas.size(); i++){
		std::cout << g_p_roadDatas[i].id << " ";
		for (int j = 0; j < g_p_roadDatas[i].channel; j++){
			list<int> temp;
			g_p_roadDatas[i].onRoadCars[0].push_back(temp);
			g_p_roadDatas[i].onRoadCars[1].push_back(temp);
		}
	}
	createCarGarage();
	//路口初始化，道路升序
	std::cout << "cross" << endl;
	for (int i = 0; i < g_p_crossDatas.size(); i++){
		std::cout << g_p_crossDatas[i].id << " ";
		for (int j = 0; j < 4; j++){
			if (-1 != g_p_crossDatas[i].roads[j]){
				g_p_crossDatas[i].orderConnectRoads.push_back(g_p_crossDatas[i].roads[j]);
			}
		}
		sort(g_p_crossDatas[i].orderConnectRoads.begin(), g_p_crossDatas[i].orderConnectRoads.end());//道路排序,id升序
	}
	std::cout << endl;
	int last_waiting_state=0;
	int last_arrived_state=0;
	while (true/* 按时间片处理 */) {
		//int flag = 0;
		std::cout << "-----new again:" << G_TIME_INDEX
		<<"\tWaiting:"<< g_no_road_waiting_state <<"  S1:"<< last_waiting_state-g_no_road_waiting_state
		<<"\tInRoad:"<< g_in_road_end_state+g_in_road_outroad_state+g_in_road_waiting_state+g_in_road_start_state
		<< "\tArrived:"<<g_no_road_arrived_state<<"  S2:"<< g_no_road_arrived_state-last_arrived_state<< endl;
		LOCK_FLAG =0;
		last_waiting_state = g_no_road_waiting_state;
		last_arrived_state = g_no_road_arrived_state;
		initState();//一轮的起点，为了判别而已
		driveJustCurrentRoad();
		//showAllCarCurrentState(timeIndex);
		driveCarInitList(G_TIME_INDEX,true);//true 优先车辆上路
		createCarSequeue();
		//showAllCarCurrentState(timeIndex);
		if(!driveCarInWaitState(G_TIME_INDEX,result)){
			result.isFail = true;
			result.lastTime = G_TIME_INDEX;
			result.T3[0]=INF;
			result.T3[1]=INF;
			result.T3[2]=INF;
			this->clear();
			return;
		}
		//showAllCarCurrentState(G_TIME_INDEX);
		if(isFinish()){
			showStatus(G_TIME_INDEX,g_p_roadDatas,1);
			getResult(result);
			result.isFail=false;
			result.lastTime = G_TIME_INDEX;
			std::cout<< "The Schedule Time is:"<< result.scheduleT<<endl;
			std::cout<< "The originResult Time is:"<< result.T_or<<endl;
			std::cout<< "The priotyResult Time is:"<< result.T_pri<<endl;
			if(isChangeGloble){
				g_crossDatas = g_p_crossDatas;
				g_roadDatas = g_p_roadDatas;
			}
			this->clear();
			return;
		}
        driveCarInitList(G_TIME_INDEX,false);
		
        updateRoadStatus(G_TIME_INDEX);
        showStatus(G_TIME_INDEX,g_p_roadDatas,1);
		if(((0 == LOCK_FLAG) && (0!=g_in_road_end_state ||0!=g_in_road_waiting_state))||(g_in_road_end_state>MAX_IN_ROAD_CAR)/* AllCarExitState(IN_ROAD_END_STATE+IN_ROAD_WAITING_STATE)*/){
			//showAllCarCurrentState(G_TIME_INDEX);//道路车辆结束
			// for(int i=0;i<g_p_carDatas.size();i++){
			// 	if(carIsState(g_p_carDatas[i],IN_ROAD_OUTROAD_STATE+IN_ROAD_WAITING_STATE)){
			// 		std::cout<<"  Car:"<<g_p_carDatas[i].id;
			// 	}
			// }
			for(int i=0;i<g_p_carDatas.size();i++){
				if(carIsState(g_p_carDatas[i],IN_ROAD_OUTROAD_STATE+IN_ROAD_WAITING_STATE)){
					std::cout<<"  Car:"<<g_p_carDatas[i].id;
				}
			}
			int fullSum=0;
			int totalRoad=0;
			for(int i=0;i<g_p_roadDatas.size();i++){
				if(g_p_roadDatas[i].isFullTime[0][G_TIME_INDEX]){
					std::cout<<"road:"<<g_p_roadDatas[i].id<< "\t"<<g_p_crossDatas.getTrueId(g_p_roadDatas[i].getfrom()) 
						<< "\t"<<g_p_crossDatas.getTrueId(g_p_roadDatas[i].getto())<<"\t"<< g_p_roadDatas[i].isDuplex
						<<"\ts"<< g_p_roadDatas[i].speed<<"\tc"<< g_p_roadDatas[i].channel<<"\tl"<< g_p_roadDatas[i].length<<"\t"<< g_p_roadDatas[i].length*g_p_roadDatas[i].channel
						<<"\tis Full"<<endl;
					fullSum++;
				}
				if(g_p_roadDatas[i].isFullTime[1][G_TIME_INDEX]){
					std::cout<<"road:"<<g_p_roadDatas[i].id<< "\t"<<g_p_crossDatas.getTrueId(g_p_roadDatas[i].getto()) 
						<< "\t"<<g_p_crossDatas.getTrueId(g_p_roadDatas[i].getfrom())<<"\t"<< g_p_roadDatas[i].isDuplex
						<<"\ts"<< g_p_roadDatas[i].speed<<"\tc"<< g_p_roadDatas[i].channel<<"\tl"<< g_p_roadDatas[i].length<<"\t"<< g_p_roadDatas[i].length*g_p_roadDatas[i].channel
						<<"\tis Full"<<endl;
					fullSum++;
				}
				totalRoad++;
				if(1==g_p_roadDatas[i].isDuplex){
					totalRoad++;
				}
			}
			cout<< "total: "<< totalRoad<<" " << "full:" <<fullSum<<" "<<fullSum*100/totalRoad<<"\%"<<endl;
			showStatus(G_TIME_INDEX,g_p_roadDatas,1);
			std::cout<< "Dead Lock! "<<endl;
			
			result.isFail = true;
			result.lastTime = G_TIME_INDEX;
			result.T3[0]=INF;
			result.T3[1]=INF;
			result.T3[2]=INF;
			this->clear();
			return;
		}
		G_TIME_INDEX++;

	}
	result.isFail = true;
	result.lastTime = G_TIME_INDEX;
	result.T3[0]=INF;
	result.T3[1]=INF;
	result.T3[2]=INF;
	this->clear();
	return;
}

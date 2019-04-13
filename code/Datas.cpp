#include "Datas.h"

unsigned int myrandom()
{
	static unsigned int RAND_SEED;
	RAND_SEED = (RAND_SEED*751+119)%65536;
	return RAND_SEED;
}
void get_V_A(){
	int carTotalNum = g_carDatas.size();
	int carPriNum = 0;

	int maxSpeed = 0;
	int minSpeed = INF;
	int priMaxSpeed = 0;
	int priMinSpeed = INF;

	int minStartTime = INF;
	int maxStartTime = 0;
	int priMinStartTime = INF;
	int priMaxEndTime=0;

	set<int> carFrom;
	set<int> priCarFrom;

	set<int> carEnd;
	set<int> priCarEnd;

	vector<vector<int>> crossFromCar;
	vector<vector<int>> priCrossFromCar;
	vector<vector<int>> crossToCar;
	vector<vector<int>> priCrossToCar;
	crossFromCar.resize(g_crossDatas.size());
	priCrossFromCar.resize(g_crossDatas.size());
	crossToCar.resize(g_crossDatas.size());
	priCrossToCar.resize(g_crossDatas.size());

	for(int i = 0;i<g_carDatas.size();i++){
		crossFromCar[g_carDatas[i].getfrom()].push_back(g_carDatas[i].id);
		crossToCar[g_carDatas[i].getto()].push_back(g_carDatas[i].id);
		maxSpeed = max(g_carDatas[i].speed,maxSpeed);
		minSpeed = min(g_carDatas[i].speed,minSpeed);
		minStartTime =  min(g_carDatas[i].planTime,minStartTime);
		maxStartTime = max(g_carDatas[i].planTime,maxStartTime);
		carFrom.insert(g_carDatas[i].getfrom());
		carEnd.insert(g_carDatas[i].getto());
		
		if(1==g_carDatas[i].priority){
			carPriNum++;
			priMaxSpeed = max(g_carDatas[i].speed,priMaxSpeed);
			priMinSpeed = min(g_carDatas[i].speed,priMinSpeed);
			priMinStartTime = min(g_carDatas[i].planTime,priMinStartTime);
			priMaxEndTime = max(g_carDatas[i].planTime,priMaxEndTime);
			priCarFrom.insert(g_carDatas[i].getfrom());
			priCarEnd.insert(g_carDatas[i].getto());
			priCrossFromCar[g_carDatas[i].getfrom()].push_back(g_carDatas[i].id);
			priCrossToCar[g_carDatas[i].getto()].push_back(g_carDatas[i].id);

		}
	}
	int maxFrom = 0;
	int minFrom = INF;
	int maxTo=0;
	int minTo=INF;
	int maxPriFrom =0;
	int minPriFrom =INF;
	int maxPriTo = 0;
	int minPriTo =INF;
	for(int i=0;i<g_crossDatas.size();i++){
		maxFrom = max(maxFrom,(int)crossFromCar[i].size());
		minFrom = min(minFrom,(int)crossFromCar[i].size());
		maxTo = max(maxTo,(int)crossToCar[i].size());
		minTo = min(minTo,(int)crossToCar[i].size());
		maxPriFrom = max(maxPriFrom,(int)priCrossFromCar[i].size());
		minPriFrom = min(minPriFrom,(int)priCrossFromCar[i].size());
		maxPriTo = max(maxPriTo,(int)crossToCar[i].size());
		minPriTo = min(minPriTo,(int)priCrossToCar[i].size());
	}
	double n1 = 0.05*carTotalNum/(double)carPriNum;
	double n2 = 0.2375*(maxSpeed/(double)minSpeed)/(priMaxSpeed/(double)priMinSpeed);
	double n3 =	0.2375*(maxStartTime/(double)minStartTime)/(priMaxEndTime/(double)priMinStartTime);
	double n4 = 0.2375*(carFrom.size()/(double)priCarFrom.size());
	double n5 = 0.2375*(carEnd.size()/(double)priCarEnd.size());
	g_v_a =  n1+ n2 + n3 + n4 + n5;
	cout.precision(10);
	cout <<"参数a:"<<g_v_a<<endl;
}
//字符串分割
void SplitString(const string& s, vector<string>& v, const char c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(std::move(s.substr(pos1, pos2 - pos1)));

		pos1 = pos2 +1;
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(std::move(s.substr(pos1)));
}
//  class CarDatas -----------------------------
void CarDatas::open(string &carPath){
	//检测
	cout << "carPath file:" << carPath << endl;
	ifstream infile;
	infile.open(carPath);
	if (!infile.is_open())
	{
		cout << "can not open carPath file:" << carPath << endl;
		getchar();
	}
	//读数据
	string line;
	
	getline(infile, line);//去除首行
	int i = 0;
	while (getline(infile, line)) // line中不包括每行的换行符  
	{
		Car C;
		sscanf(line.c_str(), "(%d,%d,%d,%d,%d,%d,%d)", &C.id, &C.truefrom, &C.trueto, &C.speed, &C.orgPlanTime, &C.priority,&C.preset);
		C.sortFlag = (((U_INT)C.priority)<<30)+(((U_INT)C.speed) << 21)+(((U_INT)(256-C.orgPlanTime))<<12)+ ((U_INT)(C.truefrom*37 + C.trueto*37+myrandom())%4096);
		C.index = i;
		C.planTime =C.orgPlanTime;
		m_idToIndex[C.id] = i;
		m_car.push_back(C);
		i++;
		//cout << "id" << m_car[i].id << " from" << m_car[i].from << " to" << m_car[i].to << " speed" << m_car[i].speed << " planTime" << m_car[i].planTime << endl;
		
	}
	cout << m_car.size() << endl;
	m_sort_by_speed = m_car;
	sort(m_sort_by_speed.begin(), m_sort_by_speed.end());
	infile.close();
}
void CarDatas::readAnswer(const string &presetAnswerPath,const string &answerPath){
	//检测
	get_V_A();
	cout << "presetAnswerPath file:" << presetAnswerPath << endl;
	ifstream infile;
	infile.open(presetAnswerPath);
	if (!infile.is_open())
	{
		cout << "can not open carPath file:" << presetAnswerPath << endl;
		getchar();
	}
	//读数据
	string line;
	char path[1000],*ch;
	vector<string> splitString;
	getline(infile, line);//去除首行
	int id, time;
	while (getline(infile, line)) // line中不包括每行的换行符  
	{
		Car C;
		memcpy(path, line.c_str()+1, line.length()-1);
		path[line.length()-2] = 0;
		ch = strtok(path, ",");
		id = atoi(ch);
		ch = strtok(NULL, ",");
		time = atoi(ch);
		cout << "id" << id << line << endl;
		g_carDatas.findById(id).planTime = time;
		ch = strtok(NULL, ",");
		while (ch != NULL){
			g_carDatas.findById(id).path_roadId.emplace_back(atoi(ch));
			ch = strtok(NULL, ",");
		}


	}
	infile.close();
	infile.open(answerPath);
		if (!infile.is_open())
		{
			cout << "can not open carPath file:" << answerPath << endl;
			getchar();
		}
		//读数据
		while (getline(infile, line)) // line中不包括每行的换行符  
		{
			memcpy(path, line.c_str()+1, line.length()-1);
			path[line.length()-2] = 0;
			ch = strtok(path, ",");
			id = atoi(ch);
			ch = strtok(NULL, ",");
			time = atoi(ch);
			cout << "id" << id << line << endl;
			g_carDatas.findById(id).planTime = time;
			ch = strtok(NULL, ",");
			if(1==g_carDatas.findById(id).preset){
				continue;
			}
			while (ch != NULL){
				g_carDatas.findById(id).path_roadId.emplace_back(atoi(ch));
				ch = strtok(NULL, ",");
			}


		}
		infile.close();
}

//       class RoadDatas-----------------------------
void RoadDatas::open(string &roadPath){
	//检测
	cout << "roadPath file:" << roadPath << endl;
	ifstream infile;
	infile.open(roadPath);
	if (!infile.is_open())
	{
		cout << "can not open roadPath file:" << roadPath << endl;
		getchar();
	}
	//读数据
	string line;
	
	getline(infile, line);//去除首行
	int i = 0;
	while (getline(infile, line)) // line中不包括每行的换行符  
	{
		Road R;
		sscanf(line.data(), "(%d,%d,%d,%d,%d,%d,%d)", &R.id, &R.length, &R.speed, &R.channel, &R.truefrom, &R.trueto, &R.isDuplex);
		R.index = i;
		m_idToIndex[R.id] = i;
		R.maxNum = R.length*R.channel;
		m_road.push_back(R);

		m_FT_2_id[(R.truefrom << 8) + R.trueto] = R.id;
		if (R.isDuplex == 1){
			m_FT_2_id[(R.trueto << 8) + R.truefrom] = R.id;
		}
		i++;
		//cout << "id" << m_road[i].id << " length" << m_road[i].length << " speed" << m_road[i].speed << " channel" << m_road[i].channel << " from" << m_road[i].from << " to" << m_road[i].to << " isDuplex" << m_road[i].isDuplex << endl;
		
	}
	firstid = m_road[0].id;
	infile.close();

}
Road& RoadDatas::findByFromTo(const int &from, const int &to){
	int truefrom = g_crossDatas[from].trueid;
	int trueto = g_crossDatas[to].trueid;
	int sum = (truefrom << 8) + trueto;
	int id = m_FT_2_id[sum];
	return findById(id);
}

//    class CrossDatas-----------------------------
void CrossDatas::open(string &crossPath){
	//检测
	cout << "crossPath file:" << crossPath << endl;
	ifstream infile;
	infile.open(crossPath);
	if (!infile.is_open())
	{
		cout << "can not open crossPath file:" << crossPath << endl;
		getchar();
	}
	//读数据
	string line;
	
	getline(infile, line);//去除首行
	int i=0;
	while (getline(infile, line)) // line中不包括每行的换行符  
	{
		Cross CR;
		sscanf(line.data(), "(%d,%d,%d,%d,%d)", &CR.trueid, &CR.roads[0], &CR.roads[1], &CR.roads[2], &CR.roads[3]);
		for (int k = 0; k < 4; k++){
			CR.roadIndex[CR.roads[k]] = k;
		}
		m_cross.push_back(CR);
		i++;
		//cout << "id" << m_cross[i].id << " north" << m_cross[i].north << " east" << m_cross[i].east << " south" << m_cross[i].south << " west" << m_cross[i].west << endl;
	}
	sort(m_cross.begin(),m_cross.end(),[](Cross cross1,Cross cross2){return cross1.trueid<cross2.trueid;});
	for(int i=0;i<m_cross.size();i++){
		m_cross[i].id = i;
		m_trueidToIndex[m_cross[i].trueid] = i;
	}
	infile.close();
}

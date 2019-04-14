//2019-4-14
//Author F.J.S.
#include "Datas.h"
#include "pantiqi.h"

CarDatas g_carDatas;
RoadDatas g_roadDatas;
CrossDatas g_crossDatas;

double g_v_a = 0;

int main(int argc, char *argv[])
{
	std::cout << "Begin" << std::endl;

	if (argc < 6){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string presetAnswerPath(argv[4]);
	std::string answerPath(argv[5]);

	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "presetAnswerPath is " << presetAnswerPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;

	// TODO:read input filebuf
	g_carDatas.open(carPath);
	g_roadDatas.open(roadPath);
	g_crossDatas.open(crossPath);
	g_carDatas.readAnswer(presetAnswerPath,answerPath);

	PanTiQi pan(g_roadDatas,g_crossDatas);
	for(int i=0;i<g_carDatas.size();i++){
		pan.push_back(g_carDatas[i].id);
	}
	Result result;
	pan.panjue(result,false);
	return 0;
}

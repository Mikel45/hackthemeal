
#include "stdafx.h"
#include "rpg.h"
#include "util.h"
#include "socket.h"

using namespace std;

cRPG rpg;

void cRPG::Initialization()
{
	vector<cRPGDecision> options;

	cRPGLocation loc("TestLoc_Entry");
	loc.AddTextParagraph(L"�� ������ ����� ����� ������ �������� �����. �� �� ������, ��� ���� ������, � ��� ������ ���������. � ��������� ������� �� ��������� ���������� ���� ���-������,\
 �� ��� ����������.");
	loc.AddTextParagraph(L"��� �������, ��� �� ������� ����� �������� ���� ���-�� �� ������. ������� ������ ���������� � ��� ���.");
	options.push_back(cRPGDecision(L"���� �� ����", "TestLoc_Center"));
	options.push_back(cRPGDecision(L"������� �����", "TestLoc_South_WallTouching"));
	loc.AttachDecisionList(options);
	RegisterLocation(loc);
	options.clear();

	loc = cRPGLocation("TestLoc_South_WallTouching");
	loc.AddTextParagraph(L"�� ���������� ����� �������, �� �� �������� ������ ���������. ��� ������ �����. ��� ������, ����� ���.");
	loc.AddTextParagraph(L"��� �������, ��� �� ������� ����� �������� ���� ���-�� �� ������. ������� ������ ���������� � ��� ���.");
	options.push_back(cRPGDecision(L"���� �� ����", "TestLoc_Center"));
	loc.AttachDecisionList(options);
	RegisterLocation(loc);
	options.clear();

	loc = cRPGLocation("TestLoc_Center");
	loc.AddTextParagraph(L"������, ����, ��� ����. �����.");
	options.push_back(cRPGDecision(L"�������������", "TestLoc_Entry"));
	loc.AttachDecisionList(options);
	RegisterLocation(loc);
	options.clear();
}

void cRPG::RegisterLocation(cRPGLocation loc)
{
	location.push_back(loc);
}

void cRPG::ParseRequest(std::string req, SOCKET* client)
{
	string args = req.substr(req.find_last_of('\n') + 1);
	string key[8];
	string value[8];
	int found = util.parseArguments(args, key, value);

	wstring response = L"";
	string fetchedLocation = "";
	// Checking where player is going
	cout << key[0] << " / " << value[0] << endl;
	if (key[0] == "location" && value[0] == "None")
	{
		fetchedLocation = "TestLoc_Entry";
	}
	else if (key[0] == "location")
	{
		for (int i = 0; i < (int)location.size(); i++)
		{
			if (location[i].guid == value[0])
			{
				fetchedLocation = location[i].decision[atoi(value[1].c_str())].linkToLocation;
				break;
			}
		}
	}
	// Searching for the location and replying
	if (fetchedLocation != "")
	{
		for (int i = 0; i < (int)location.size(); i++)
		{
			if (location[i].guid == fetchedLocation)
			{
				response = L"guid=" + wstring(location[i].guid.begin(), location[i].guid.end()) + L"%%";
				response += L"description=" + location[i].text + L"%%";
				response += L"decCount=" + to_wstring((int)location[i].decision.size()) + L"%%";
				for (int y = 0; y < (int)location[i].decision.size(); y++)
				{
					response += location[i].decision[y].text;
					if (y < (int)location[i].decision.size() - 1)
						response += L"%%";
				}
				break;
			}
		}
	}
	// Send
	if (response.length() > 0)
	{
		sock.sendDataToJS(response, client);
	}
}
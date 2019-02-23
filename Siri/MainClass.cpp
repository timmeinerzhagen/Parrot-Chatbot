#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>


const int MAX_V_1 = 8; // [X][ ][ ] - groesse von source
const int MAX_V_2 = 2; // [ ][X][ ] - groesse von source
const int MAX_V_3 = 7; // [ ][ ][X] - groesse von source

/*
 * Die Antworten aus dem Array source in das array dest uebertragen	
 * @source Die zu uebertragene Array
 * @dest Der Ziehlort fuer das Array
 */
static void load(std::string source[MAX_V_1][MAX_V_2][MAX_V_3], std::vector<std::vector<std::vector<std::string>>>* dest){
	for (int i = 0; i < MAX_V_1; i++){
		dest->push_back(std::vector<std::vector<std::string>>());
		for (int e = 0; e < MAX_V_2; e++){
			dest->at(i).push_back(std::vector<std::string>());
			for (int z = 0; z < MAX_V_3; z++){
				if (source[i][e][z] == "/")
					break;
				dest->at(i).at(e).push_back(source[i][e][z]);
			}
		}
	}
}

int main(){

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Source Array

	std::string source[MAX_V_1][MAX_V_2][MAX_V_3] = { 
		{ { " ", "/" },
			{ "Darauf habe ich leider keine Antwort. Bitte formulieren Sie die Frage um.", "/" } },
		{ { "heisst", "du", "name", "/" } ,
			{ "Siri", "/" } },
		{ { "alt", "du", "/" },
			{ "Keine Ahnung.", "/" } },
		{ { "bist", "du", "/" },
			{ "Eine künstliche Intelligenz.", "/" } },
		{ { "deine", "lieblingsfarbe", "/" },
			{ "grün", "rot", "himmelblau", "gelb", "schwarz", "blöde Frage", "/" } },
		{ { "dein ", "lieblingstier", "/" },
			{ "Hase", "Huhn", "Tiger", "Elefant", "Giraffe", "Ich kann nichts mit Tieren anfangen.", "/" } },
		{ { "machen", "wir", "abend", "/" },
			{ "Genau das selbe, wie jeden Abend. Wir versuchen, die Weltherrschaft an uns zu reißen.", "/" } },
		{ { "hallo", "tag", "/" },
			{ "Hallo", "Wie geht es dir?", "/" } },
	};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Array anfuegen
	std::vector<std::vector<std::vector<std::string>>> antworten = std::vector<std::vector<std::vector<std::string>>>();
	load(source, &antworten);

	std::cout << "Du:" << std::endl;
	std::cout << "  ";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Main Loop
	std::string s = "-"; // eingabe kleinbuchstaben
	std::string cS = "-"; // eingabe Origimal
	std::string ant = "-"; // letzte Antwort des Computers
	int pos;
	do{
		cS = " ";
		std::getline(std::cin, cS);
		s = cS;
		//s.replace(s.begin(), s.end(), 'ß', 'ss');
		std::transform(cS.begin(), cS.end(), s.begin(), ::tolower);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Cmds
		if (s == "clear"){ // cmd clearen ("clear <>")
			system("cls");
			std::cout << "<< Cmd geleert >>" << std::endl;
			std::cout << std::endl << "Computer:" << std::endl;
			std::cout << "  " << ant << std::endl;
			std::cout << std::endl << "Du:" << std::endl;
			std::cout << "  ";
			continue;
		}else if (s.substr(0, 4) == "show"){ // zugehoerige Antworten zeigen ("show" / "show <Frage>")
			if (s.size() == 4){ // Alle Antworten
				for (int i = 1; i < antworten.size(); i++){
					for (int e = 0; e < antworten[i][0].size(); e++){
						std::cout << "  <-" << antworten[i][0][e] << std::endl;
					}
					for (int e = 0; e < antworten[i][1].size(); e++){
						std::cout << "    >-" << antworten[i][1][e] << std::endl;
					}
				}
			}else{ // Spezifische Antwort
				for (int i = 1; i < antworten.size(); i++){
					for (int e = 0; e < antworten[i][0].size(); e++){
						if (antworten[i][0][e].find(s.substr(5, s.size() - 1)) != 4294967295){
							for (int e = 0; e < antworten[i][1].size(); e++){
								std::cout << "    >-" << antworten[i][1][e] << std::endl;
							}
							i = antworten.size(); 
							break;
						}
					}
				}
			}
			std::cout << std::endl << "Du:" << std::endl;
			std::cout << "  ";
			continue;
		}else if (s.substr(0, 6) == "reload"){ // Antworten neu laden ("reload <>")
			antworten.clear();
			load(source, &antworten);
			system("cls");
			ant = "-";
			std::cout << "<< Neu Geladen >>" << std::endl;
			std::cout << std::endl << "Du:" << std::endl;
			std::cout << "  ";
			continue;
		}
		
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Antworterweiterung
		if (ant != "-"){
			std::transform(ant.begin(), ant.end(), ant.begin(), ::tolower);
			std::istringstream a(ant);
			std::vector<std::string> sa{ std::istream_iterator<std::string>{a}, std::istream_iterator<std::string>{} };

			int c = 0, position = 0;
			for (int i = 1; i < antworten.size(); i++){
				int match = 0;
				for (int e = 0; e<antworten[i][0].size(); e++){
					for each(std::string str in sa){
						if (antworten[i][0][e].find(str) != 4294967295){
							++match; 
						}
						//std::cout << " |-| " << antworten[i][0][e] << " + " << s << " -|-" << (antworten[i][0][e].find(s) != 4294967295) << std::endl;
					}
				}
				if (match > c){
					position = i;
					c = match;
				}
			}
			std::vector<std::string> sb = std::vector<std::string>();
			sb.push_back(cS);
			if (c > 0){ // Schon existente Frage
				bool b = true;
				for each(std::string comp in antworten.at(position).at(1)){
					if (comp == cS) b = false;
				} if (b){
					antworten.at(position).at(1).insert(antworten.at(position).at(1).end(), sb.begin(), sb.end());
				}
			}
			else{ // Neue Frage + Antwort
				antworten.push_back(std::vector<std::vector<std::string>>());
				antworten.at(antworten.size() - 1).push_back(sa);
				antworten.at(antworten.size() - 1).push_back(sb);
			}
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Passende Antwort raussuchen
		pos = 0;
		int count = 0;
		for (int i = 1; i < antworten.size(); i++){
			int match = 0;
			for (int e = 0; e<antworten[i][0].size(); e++){
				//std::cout << match << " - " << i << " - " << match << " - " << count << " -- " << (s.find(antworten[i][0][e]) != 4294967295) << " - " << antworten[i][0][e];
				if (s.find(antworten[i][0][e]) != 4294967295)
					++match;// std::cout << " |-| " << ++match << antworten[i][0][e];
				//std::cout << std::endl;
			}
			if (match > count){
				pos = i;
				count = match;
			}
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Ausgabe
		std::cout << "Computer:" << std::endl;
		ant = " ";
		ant = antworten[pos][1][rand() % antworten[pos][1].size()]; // Bei mehreren Antwortmoeglichkeiten eine per Zufall waehlen
		std::cout << "  " << ant << std::endl;
		std::cout << std::endl << "Du:" << std::endl;
		std::cout << "  ";

	} while (s!="exit"); // bei exit Programm beenden
	return 0;
}



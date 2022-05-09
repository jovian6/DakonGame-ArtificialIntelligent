#include<sfml\Graphics.hpp>
#include<iostream>
#include<sstream>
#include<windows.h>
#include<time.h>
#include<stdlib.h>
using namespace std;

class EllipseShape : public sf::Shape 
{
public:

	explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0)) :
		m_radius(radius)
	{
		update();
	}

	void setRadius(const sf::Vector2f& radius)
	{
		m_radius = radius;
		update();
	}

	const sf::Vector2f& getRadius() const
	{
		return m_radius;
	}

	virtual unsigned int getPointCount() const
	{
		return 30; // fixed, but could be an attribute of the class if needed
	}

	virtual sf::Vector2f getPoint(unsigned int index) const
	{
		static const float pi = 3.141592654f;

		float angle = index * 2 * pi / getPointCount() - pi / 2;
		float x = std::cos(angle) * m_radius.x;
		float y = std::sin(angle) * m_radius.y;

		return sf::Vector2f(m_radius.x + x, m_radius.y + y);
	}

private:

	sf::Vector2f m_radius;
}; 

void gotoxy(int x, int y) { 
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class Dakon {
protected:

#pragma region VARIABEL UNTUK DISPLAY //?
	sf::Text isijalan[16]; 
	sf::Text currentpoint;
	sf::Text menu;
	sf::Text sisajalan;
	sf::Text pilihanindex;
	sf::Text giliran[2];
	sf::Text pemenang;
	sf::Font fonts;

	sf::Vector2f radiusboardutama;
	sf::Vector2f posboardutama;
	sf::Vector2f posjalan;

	EllipseShape boardutama;
	sf::CircleShape jalan_gambar[16];
	int panjanglayar, lebarlayar;
#pragma endregion

	int jalan[16];
	int player;
public:
	Dakon() {
		for (int i = 0; i < 16; i++) {
			jalan[i] = 7;   //0 - 6 : JALAN AI
		}					//8 - 14: JALAN PLAYER
		jalan[7] = 0;  //LUMBUNG AI
		jalan[15] = 0; //LUMBUNG PLAYER

		#pragma region VARIABEL UNTUK DISPLAY
		panjanglayar = 1000;
		lebarlayar = 400;
		if (!fonts.loadFromFile("C:/Users/User/Downloads/PercobaanUAS_Akhir/font1.ttf")) //LOAD FONT
		{
			std::cout << "Error loading font\n";
		}

		//SET TEXT PILIH INDEX 
		menu.setString("Index: ");
		menu.setFont(fonts);
		menu.setFillColor(sf::Color::White);
		menu.setCharacterSize(30);
		menu.setPosition(50.0f, 50.0f);

		//SET TEXT JUMLAH DAKON YANG SEDANG BERJALAN
		sisajalan.setString("Sisa Jalan: ");
		sisajalan.setFont(fonts);
		sisajalan.setFillColor(sf::Color::White);
		sisajalan.setCharacterSize(30);
		sisajalan.setPosition(500.0f, 50.0f);

		//SET TEXT PEMENANG
		pemenang.setFont(fonts);
		pemenang.setFillColor(sf::Color::White);
		pemenang.setCharacterSize(30);
		pemenang.setPosition(400.0f, 50.0f);

		//SET TEXT INDEX YG DIPILIH
		currentpoint.setFont(fonts);
		currentpoint.setFillColor(sf::Color::White);
		currentpoint.setCharacterSize(30);
		currentpoint.setPosition(650.0f, 50.0f);

		//SET TEXT GILIRAN
		for (int i = 0; i < 2; i++) {
			if (i == 0) {
				giliran[i].setString("Giliran AI");
			}
			else {
				giliran[i].setString("Giliran User");
			}
			giliran[i].setFont(fonts);
			giliran[i].setFillColor(sf::Color::White);
			giliran[i].setCharacterSize(30);
			giliran[i].setPosition(50.0f, 10.0f);
		}

		//SET TEXT PILIHAN
		pilihanindex.setFont(fonts);
		pilihanindex.setFillColor(sf::Color::White);
		pilihanindex.setCharacterSize(30);
		pilihanindex.setPosition(150.0f, 50.0f);

		//SET POSISI DAN GAMBAR BOARD UTAMA LONJONG
		radiusboardutama.x = 400.0f; //jari2 X
		radiusboardutama.y = 100.0f; //jari2 Y
		posboardutama.x = panjanglayar / 2.0f; //posisi x
		posboardutama.y = lebarlayar / 2.0f; //posisi y
		boardutama.setRadius(sf::Vector2f(radiusboardutama)); //di set
		boardutama.setOrigin(radiusboardutama);
		boardutama.setFillColor(sf::Color::Blue);
		boardutama.setPosition(posboardutama);

		int scoreawal = 0;
		stringstream ss;
		scoreawal = 7;
		ss << scoreawal;

		// SET ISI TEXT dan NILAI LUMBUNG
		for (int i = 0; i < 16; i++) {
			isijalan[i].setString(ss.str());
			isijalan[i].setFont(fonts);
			isijalan[i].setFillColor(sf::Color::Black);
			isijalan[i].setCharacterSize(30);
		}

		//POSISI DAN GAMBAR "LUMBUNG" AI n PLAYER
		for (int j = 0; j < 2; j++) {
			int i;
			if (j == 0) {
				i = 7;
			}
			else {
				i = 15;
			}
			jalan_gambar[i].setRadius(radiusboardutama.y / 2.5f);
			jalan_gambar[i].setFillColor(sf::Color::Green);
			if (i == 7) {
				jalan_gambar[i].setPosition(posboardutama.x - radiusboardutama.x + 50.0f, radiusboardutama.y + radiusboardutama.y / 2.0f + 10.0f);
				isijalan[i].setPosition(posboardutama.x - radiusboardutama.x + 80.0f, radiusboardutama.y + radiusboardutama.y / 2.0f + 30.0f);
			}
			else {
				jalan_gambar[i].setPosition(posboardutama.x * 2.0f - (posboardutama.x - radiusboardutama.x) - 135.0f, radiusboardutama.y + radiusboardutama.y / 2.0f + 10.0f);
				isijalan[i].setPosition(posboardutama.x * 2.0f - (posboardutama.x - radiusboardutama.x) - 105.0f, radiusboardutama.y + radiusboardutama.y / 2.0f + 30.0f);
			}
		}

		//POSISI DAN GAMBAR "JALAN" AI n PLAYER
		posjalan.x = posboardutama.x - radiusboardutama.x + (radiusboardutama.x * 2.0f / 7.0f);
		posjalan.y = radiusboardutama.y + 30.0f;
		for (int i = 0; i < 16; i++) {
			if (i != 7 && i != 15) {
				jalan_gambar[i].setRadius(radiusboardutama.y / 3.0f);
				jalan_gambar[i].setFillColor(sf::Color::Cyan);
				if (i == 8) {
					posjalan.x = posboardutama.x - radiusboardutama.x + (radiusboardutama.x * 2.0f / 7.0f);
					posjalan.y += radiusboardutama.y - 25.0f;
				}
				if (i != 0 && i != 8) {
					posjalan.x += (radiusboardutama.x * 2.0f / 10.5f);
				}
				jalan_gambar[i].setPosition(posjalan.x + 20.0f, posjalan.y);
				isijalan[i].setPosition(posjalan.x + 40.0f, posjalan.y + 15.0f);
			}
		}
#pragma endregion
	}

	#pragma region set get
	void set_player(int ply) { //PLAYER 1 = AI & PLAYER 2 = PLAYER
		player = ply;
	}
	int get_player() {
		return player;
	}
	int get_value(int index) {
		return jalan[index];
	}
	int get_lumbung1() {
		return jalan[7];
	}
	int get_lumbung2() {
		return jalan[15];
	}
#pragma endregion

	bool wincheck() {
		if (jalan[7] + jalan[15] == 7 * 14) {
			return false;
		}
		return true; 
	}
	void copy(Dakon ctr) { //COPY KONDISI TERAKHIR
		for (int i = 0; i < 16; i++)
		{
			jalan[i] = ctr.jalan[i];
		}
	}

	void draw(sf::RenderWindow& window, int player, int temp = 0, int index = 0) { //DRAW
		int tempindex = 14;
		//UPDATE ANGKA
		for (int i = 0; i < 16; i++) {
			stringstream s;
			s << jalan[i];
			if (i > 7 && i != 15) {
				isijalan[tempindex--].setString(s.str());
			}
			else if(i==7) {
				isijalan[15].setString(s.str());
			}
			else if (i == 15) {
				isijalan[7].setString(s.str());
			}
			else {
				isijalan[i].setString(s.str());
			}
		}
		stringstream s;
		s << temp;
		currentpoint.setString(s.str());
		stringstream ss;
		//UPDATE BIJI
		if (index == 0) {
			pilihanindex.setString("?");
		}
		else {
			ss << index;
			pilihanindex.setString(ss.str());
		}

		//UPDATE GAMBAR
		window.clear();
		window.draw(boardutama);
		for (int i = 0; i <16; i++) {
			window.draw(jalan_gambar[i]);
			window.draw(isijalan[i]);
		}
		if (temp != 0) {
			window.draw(sisajalan);
			window.draw(currentpoint);
			window.draw(menu);
			window.draw(pilihanindex);
		}
		window.draw(giliran[player-1]);
		window.display();
	}
	bool check(int ply) { //CEK APAKAH SATU DERET JALAN (0-6)/(8-14) KOSONG 
		if (ply == 1) {
			int x = 0;
			for (int i = 0; i < 7; i++) {
				if (get_value(i) == 0) {
					x++;
				}
			}
			if (x == 7) {
				return true;
			}
		}
		if (ply == 2) {
			int x = 0;
			for (int i = 8; i < 15; i++) {
				if (get_value(i) == 0) {
					x++;
				}
			}
			if (x == 7) {
				return true;
			}
		}
		return false;
	}
	void run(sf::RenderWindow &window, int player, int index, int AI = 0) {
		int indexawal = index;
		int sl = 400; //SLEEP
		if (player == 1 && (index >= 0 && index <= 6) && get_value(index) != 0) { //AI
			int temp = get_value(index); //JUMLAH BIJI YANG DIAMBIL DI JALAN YANG DIPILIH
			jalan[index] = 0; //JUMLAH BIJI DI JALAN TERSEBUT DI 0 KAN
			while (temp > 0) //SELAMA BIJI BELUM HABIS, AI TERUS BERMAIN
			{
				if (index == 14) { //PENGECUALIAN SPY GA ISI index = 15 
					index = -1; //DIBUAT -1 SUPAYA SELANJUTNYA LGSG MENGISI index = 0;
				}
				if (index != 15) { //KARENA LUMBUNG PLAYER DI index = 15 
					index++;
					jalan[index] += 1;
					temp--;
					if (AI != 1) { //JIKA AI == 0 MAKA MASUK IF UTK DRAW
						draw(window, player, temp, indexawal);
						Sleep(sl);
					}
				}
				if (temp == 0 && jalan[index] > 1 && index != 7 && index != 15) { //JIKA BIJI HABIS DAN BERHENTI DI JALAN YANG MEMILIKI JUMLAH BIJI > 1 																		
					temp = jalan[index];									      //MAKA AKAN DIAMBIL LAGI UTK MELANAJUTKAN PERMAINAN
					if (AI != 1) { //JIKA AI == 0 MAKA MASUK IF UTK DRAW
						draw(window, player, temp, indexawal);
						Sleep(sl);
					}
					jalan[index] = 0; //JUMLAH BIJI DI JALAN TERSEBUT DI 0 KAN
				}
			}
			if (get_value(index) == 1 && (index >= 0 && index <= 6) && jalan[14 - index] != 0) { //AI MENEMBAK
				jalan[7] += jalan[index] + jalan[14 - index]; //14 - index ITU RUMUS BUAT DAPET INDEX SEBERANGE
				jalan[index] = 0; //YANG MENEMBAK MASUK LUMBUNG
				jalan[14 - index] = 0; //YANG DITEMBAK MAUSK LUMBUNG
				if (AI != 1) { //JIKA AI == 0 MAKA MASUK IF UTK DRAW
					draw(window, player, temp, indexawal);
					Sleep(sl);
				}
				set_player(2); //SETELAH MENEMBAK GANTI PLAYER
			}
			else if (get_value(index) == 1 && (index >= 0 && index <= 6) && jalan[14 - index] == 0) { //AI MENEMBAK KOSONGAN MAKA KONDISI TIDAK BERUBAH DAN GANTI PLAYER
				set_player(2); //SETELAH MENEMBAK KOSONGAN GANTI PLAYER
			}
			else if (index == 7) { //BERHENTI DI LUMBUNG AI
				set_player(1); //AI AKAN BERMAIN LAGI
			}
			else if (get_value(index) == 1 && (index >= 8 && index <= 15)) { //JIKA BIJI TERAKHIR BERHENTI DI JALAN PLAYER YANG KOSONG
				set_player(2); //GANTI PLAYER
			}
		}
		else if (player == 2 && (index >= 8 && index <= 14) && get_value(index) != 0) { //PLAYER
			int temp = get_value(index);
			jalan[index] = 0;
			while (temp > 0)
			{
				if (index == 15) { //SETELAH MENGISI LUMBUNG SENDIRI AKAN DI -1 SUPAYA JALAN SELANJUNTYA MENGISI index = 0
					index = -1;
				}
				if (index != 6) { //SUPAYA TIDAK MENGISI index = 7 (LUMBUNG AI)
					index++;
					jalan[index] += 1;
					temp--;
					if (AI != 1) {
						draw(window, player, temp, indexawal);
						Sleep(sl);
					}
				}
				if (index == 6) {
					if (get_value(index) == 1) {
						set_player(1);
					}
					index = 7; // STLH MENGISI index = 6 LGSG DIBUAT index = 7 SUPAYA JALAN SELANJUTNYA MENGISI index = 8
				}
				if (temp == 0 && jalan[index] > 1 && index != 15 && index != 7) { //JIKA BIJI HABIS DAN BERHENTI DI JALAN YANG MEMILIKI JUMLAH BIJI > 1 
					temp = jalan[index];											//MAKA AKAN DIAMBIL LAGI UTK MELANAJUTKAN PERMAINAN
					if (AI != 1) {
						draw(window, player, temp, indexawal);
						Sleep(sl);
					}
					jalan[index] = 0;//JUMLAH BIJI DI JALAN TERSEBUT DI 0 KAN
				}
				if (temp == 0 && jalan[index-1] > 1 && index == 7) { //JIKA BIJI HABIS DAN BERHENTI DI JALAN YANG MEMILIKI JUMLAH BIJI > 1 
					temp = jalan[6];								 //tetapi di index == 6 maka mengambil idx 6
					if (AI != 1) {
						draw(window, player, temp, indexawal);
						Sleep(sl);
					}
					jalan[6] = 0;
				}
			}
			if (get_value(index) == 1 && (index >= 8 && index <= 14) && jalan[14 - index] != 0) { //PLAYER MENEMBAK
				jalan[15] += jalan[index] + jalan[14 - index];
				jalan[index] = 0;
				jalan[14 - index] = 0;
				if (AI != 1) {
					draw(window, player, temp, indexawal);
					Sleep(sl);
				}
				set_player(1);
			}
			else if (get_value(index) == 1 && (index >= 8 && index <= 14) && jalan[14 - index] == 0) { //PLAYER MENEMBAK JALAN KOSONG
				set_player(1);
			}
			else if (index == 15) { //BERHENTI DI LUMBUNG PLAYER
				set_player(2);
			}
			else if (get_value(index) == 1 && (index >= 0 && index <= 6)) { //BERHENTI DI JALAN AI
				set_player(1);
			}
		}
	}
	void finish(sf::RenderWindow &window) {
		if (jalan[7] > jalan[15]) {
			pemenang.setString("AI Won!");
		}
		else if (jalan[7] < jalan[15]) {
			pemenang.setString("Player Won!");
		}
		else {
			pemenang.setString("It's a tie!");
		}
		window.clear();
		window.draw(pemenang);
		window.display();
		Sleep(2500);
		exit(0);
	}
};

class Pruning {
protected:
	Dakon ctr;
	bool cond; // UNTUK LOOP NUNGGU PLAYER INPUT ANGKA
	int indexplayer;
public:
	Pruning() { cond = true; }

	void input() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
			indexplayer = 14;
			cond = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
			indexplayer = 13;
			cond = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
			indexplayer = 12;
			cond = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
			indexplayer = 11;
			cond = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {
			indexplayer = 10;
			cond = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) {
			indexplayer = 9;
			cond = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) {
			indexplayer = 8;
			cond = false;
		}
	}

	int run_data(sf::RenderWindow &window) {
		Dakon save, save1, save2, save3, save4, save5, save6;

		save6.copy(ctr);
		int keep_e = -10000, data5 = 10000, e_s = 0, e_l = 6;
		while (ctr.get_value(e_s) == 0 && e_s < 7) { e_s++; }
		while (ctr.get_value(e_l) == 0 && e_l > -1) { e_l--; }
		int op = e_s;
		for (int e = e_s; e < e_l + 1; e++) //AI
		{
			if (ctr.get_value(e) != 0)
			{
				ctr.run(window, 1, e, 1);
				save5.copy(ctr);

				int keep_f = 10000, data4 = -10000, f_s = 8, f_l = 14;
				while (ctr.get_value(f_s) == 0 && f_s < 15) { f_s++; }
				while (ctr.get_value(f_l) == 0 && f_l > 7) { f_l--; }
				if (f_l == 8) { // 0 0 0 0 1 0 0
					op = e;
					return op;
				}
				for (int f = f_s; f < f_l + 1; f++) //PLAYER
				{
					if (ctr.get_value(f) != 0)
					{
						ctr.run(window, 2, f, 1);
						save4.copy(ctr);

						int keep_g = -10000, data3 = 10000, g_s = 0, g_l = 6;
						while (ctr.get_value(g_s) == 0 && g_s < 7) { g_s++; }
						while (ctr.get_value(g_l) == 0 && g_l > -1) { g_l--; }
						for (int g = g_s; g < g_l + 1; g++) //AI
						{
							if (ctr.get_value(g) != 0)
							{
								ctr.run(window, 1, g, 1);
								save3.copy(ctr);

								int keep_h = 10000, data2 = -10000, h_s = 8, h_l = 14;
								while (ctr.get_value(h_s) == 0 && h_s < 15) { h_s++; }
								while (ctr.get_value(h_l) == 0 && h_l > 7) { h_l--; }
								for (int h = h_s; h < h_l + 1; h++) //PLAYER
								{
									if (ctr.get_value(h) != 0)
									{
										ctr.run(window, 2, h, 1);
										save2.copy(ctr);

										int keep_i = -10000, data1 = 10000, i_s = 0, i_l = 6;
										while (ctr.get_value(i_s) == 0 && i_s < 7) { i_s++; }
										while (ctr.get_value(i_l) == 0 && i_l > -1) { i_l--; }
										for (int i = i_s; i < i_l + 1; i++) //AI
										{
											if (ctr.get_value(i) != 0)
											{
												ctr.run(window, 1, i, 1);
												save1.copy(ctr);

												int keep_j = 10000, j_s = 8, j_l = 14;
												while (ctr.get_value(j_s) == 0 && j_s < 15) { j_s++; }
												while (ctr.get_value(j_l) == 0 && j_l > 7) { j_l--; }
												for (int j = j_s; j < j_l + 1; j++) //PLAYER
												{
													if (ctr.get_value(j) != 0)
													{
														ctr.run(window, 2, j, 1);
														{
															int lumbung = ctr.get_lumbung1() - ctr.get_lumbung2();
															if (i == i_s) { 
																data1 = min(data1, lumbung);
															}
															else if (i != i_s) {
																if (lumbung <= data1) {
																	j = 15; //break
																}
																else {
																	if (keep_j > lumbung) {
																		keep_j = lumbung;
																	}
																}
																if (j == j_l) {
																	data1 = keep_j;
																}
															}
														}
													}
													ctr.copy(save1);
												}
												if (h == h_s) {
													data2 = max(data2, data1);
												}
												else if (h != h_s) {
													if (data1 >= data2) {
														i = 7;
													}
													else {
														if (keep_i < data1) {
															keep_i = data1;
														}
													}
													if (i == i_l) {
														data2 = keep_i;
													}
												}
											}
											ctr.copy(save2);
										}
										if (g == g_s) {
											data3 = min(data3, data2);
										}
										else if (g != g_s) {
											if (data2 <= data3) {
												h = 15;
											}
											else {
												if (keep_h > data2) {
													keep_h = data2;
												}
											}
											if (h == h_l) {
												data3 = keep_h;
											}
										}
									}
									ctr.copy(save3);
								}
								if (f == f_s) {
									data4 = max(data4, data3);
								}
								else if (f != f_s) {
									if (data3 >= data4) {
										g = 7;
									}
									else {
										if (keep_g < data3) {
											keep_g = data3;
										}
									}
									if (g == g_l) {
										data4 = keep_g;
									}
								}
							}
							ctr.copy(save4);
						}
						if (e == e_s) {
							data5 = min(data5, data4);
							op = e;
						}
						else if (e != e_s) {
							if (data4 <= data5) {
								f = 15;
							}
							else {
								if (keep_f > data4) {
									keep_f = data4;
									op = e;
								}
							}
							if (f == f_l) {
								data5 = keep_f;
							}
						}
					}
					ctr.copy(save5);
				}
			}
			ctr.copy(save6);
		}
		return op;
	}

	void play(sf::RenderWindow& window) {
		while (ctr.wincheck())
		{
			if (ctr.check(1)) { //JIKA SELURUH JALAN AI KOSONG MAKA LGSG GANTI PLAYER
				ctr.draw(window, 2);
				cout << "Input Index: ";
				while (cond) {
					input();
				}
				cond = true;
				ctr.run(window, 2, indexplayer);
			}
			else if (ctr.check(2)) { //JIKA SELURUH JALAN PLAYER KOSONG MAKA LGSG GANTI AI
				ctr.draw(window, 1);
				cout << run_data(window) << endl;
				ctr.run(window, 1, run_data(window));
			}
			else if (ctr.get_player() == 1) { //GILIRAN AI
				ctr.draw(window, 1);
				cout << run_data(window) << endl;
				ctr.run(window, 1, run_data(window));
			}
			else { //GILIRAN PLAYER
				ctr.draw(window, 2);
				cout << "Input Index: ";
				while (cond) {
					input();
				}
				cond = true;
				ctr.run(window, 2, indexplayer);
			}
		}
		ctr.finish(window);
	}
};

int main() {
	Pruning d;
	sf::RenderWindow window(sf::VideoMode(1000, 400), "Congklak Sederhana", sf::Style::Default);
	while (window.isOpen())
	{
		d.play(window);
	}
	return 0;
}
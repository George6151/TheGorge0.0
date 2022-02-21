#include <iostream>
#include <ctime>
#include <fstream>
#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
class Game
{
private:
	//------------------------------Game data---------------------------------//
	bool state_menu = false, state_load = false;//Game menu validation
	bool state_mines = false, state_buymines = false, state_confirmPurchase = false;//Mine menu validation
	bool state_storage = false, state_buystorages = false;//storage menu validation
	string menu_selection, load_selection, token;//Game variables
	string buymines_selection, mines_selection, confirm_purchase;//Mine input variables
	string buystorage_selection, storage_selection;//Storage input varables
	int collection_time = 0, collection_amount = 0, mine_capacity_MAX = 0;//Collection processing and validation
	double upgrade_price = 0, indice_number = 0, collection_rate = 0;//Exponential function values
	int day = 0, width = 10;
	//------------------------------Save data---------------------------------//
	int gold = 10000000, saved_time = 0, current_time = 0;//Time and Money
	int storage[4]{ 0,0,0,0 };//Storages
	int mines[10]{ 1,2,3,4,5,6,7,8,9,10 };//Mines
	int mine_saving_var = 0, storage_saving_var = 0;//Variables used in the save the arrays 
	const int mines_MAX = 10, storage_MAX = 10;//Maximum level for the 
public:
	string findDay()
	{
		string week_day;
		refreshGame();
		day = (current_time / 86400) % 7;
		if (day == 1)
		{
			week_day = "Friday";
		}
		if (day == 2)
		{
			week_day = "Saturday";
		}
		if (day == 3)
		{
			week_day = "Sunday";
		}
		if (day == 4)
		{
			week_day = "Monday";
		}
		if (day == 5)
		{
			week_day = "Tuesday";
		}
		if (day == 6)
		{
			week_day = "Wednesday";
		}
		if (day == 0)
		{
			week_day = "Thursday";
		}
		return week_day;
	}//end of function
	//Main Menu
	void mainMenu()
	{
		refreshGame();
		while (state_menu == false)
		{
			cout << "Options - Please select an option" <<setw(width)<<"Gold: " <<gold<< endl << "1) Create a Save" << endl << "2) Load a Save" << endl << "3) Save your game" << endl << "4) Refresh" << endl << "5) Exit" << endl << "Selection: ";
			cin >> menu_selection;
			if (menu_selection == "1")
			{
				createGame();
			}
			else if (menu_selection == "2")
			{
				loadGame();
			}
			else if (menu_selection == "3")
			{
				saveGame();
			}
			else if (menu_selection == "4")
			{
				refreshGame();
			}
			else if (menu_selection == "5")
			{
				state_menu = true;
			}
			else
			{
				cout << "Invalid selection" << endl;
			}
		}//end of while
		state_menu = false;
	}//end if function
	void createGame()
	{
		ofstream creation("Save.txt");
		creation.close();
		cout << "Game has successfully been Created!" << endl;
	}//end of function
	void saveGame()
	{
		ofstream save("Save.txt");
		refreshGame();
		saved_time = current_time;
		save << saved_time << endl << gold << endl;
		for (int save_mines = 0; save_mines < 10; save_mines++)
		{
			mine_saving_var = mines[save_mines];
			save << mine_saving_var << endl;
		}
		for (int save_stroages = 0; save_stroages < 4; save_stroages++)
		{
			storage_saving_var = storage[save_stroages];
			save << storage_saving_var << endl;
		}
		save.close();
		cout << "Game has successfully been Saved!" << endl;
	}//end of function
	void loadGame()
	{
		while (state_load == false)
		{
			cout << "Are you sure you want to load into a different save?" << endl << "1) Yes" << endl << "2) No" << endl << "Selection: ";
			cin >> load_selection;
			if (load_selection == "1")
			{
				int loading_array[16] = { 0 };
				int loading_loaction = 0;
				ifstream load("Save.txt");
				while (load >> token)
				{
					stringstream string_load(token);
					int string_capture = 0;
					string_load >> string_capture;
					cout << string_capture << endl;
					loading_array[loading_loaction] = string_capture;
					loading_loaction++;
				}//end of while
				saved_time = loading_array[0];
				gold = loading_array[1];

				for (int load_mines = 0; load_mines < 10; load_mines++)
				{
					mines[load_mines] = loading_array[load_mines + 2];
				}

				for (int load_storages = 0; load_storages < 4; load_storages++)
				{
					storage[load_storages] = loading_array[load_storages + 12];
				}
				cout << "Load Complete!" << endl;
				state_load = true;
			}//end of if
			else if (load_selection == "2")
			{
				state_load = true;
			}//end of else if 
			else
			{
				cout << "Invalid selection" << endl;
			}//end of else
		}//end of while
		state_load = false;
	}//end of function
	void refreshGame()
	{
		current_time = time(NULL);
	}
	//Money Handeling
	//Mines
	void menuMines()
	{
		while (state_mines == false)
		{
			showMines();
			cout << "Mines - Please select an option" << setw(width) << "Gold: " << gold << endl << "1) Buy / Upgrade Mines" << endl << "2) Collect from Mines" << endl << "3) Back" << endl << "Selection: ";
			cin >> mines_selection;
			if (mines_selection == "1")
			{
				buyMinesMenu();
			}//end of else if
			else if (mines_selection == "2")
			{
				collectMines();
			}//end of else if
			else if (mines_selection == "3")
			{
				state_mines = true;
			}//end of else if
			else
			{
				cout << "Invalid seleciton!" << endl;
			}//end of else
		}//end of while
		state_mines = false;
	}//end of fucntion
	void showMines()
	{
		calculateCollection();
		cout << "You earn " << collection_rate << " gold a second, with a maximum mine capacity of " << mine_capacity_MAX << " gold" << endl;
		for (int minecount = 0; minecount < 10; minecount++)
		{
			cout << "Mine " << minecount + 1 << " is Lv." << mines[minecount] << endl;
		}//end of for
	}//end of function
	void buyMinesMenu()
	{
		while (state_buymines == false)
		{
			showMines();
			cout << "Which Mine would you like to upgrade?" << endl << "Selection: ";
			cin >> buymines_selection;
			if (buymines_selection == "1")
			{
				if (mines[0] > 10)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of else if
				else
				{
					buyMines(mines[0]);
				}//end of else
			}//end of if
			else if (buymines_selection == "2")
			{
				if (mines[1] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of else if
				else
				{
					buyMines(mines[1]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "3")
			{
				if (mines[2] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of else if
				else
				{
					buyMines(mines[2]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "4")
			{
				if (mines[3] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of else if
				else
				{
					buyMines(mines[3]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "5")
			{
				if (mines[4] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of else if
				else
				{
					buyMines(mines[4]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "6")
			{
				if (mines[5] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of else if
				else
				{
					buyMines(mines[5]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "7")
			{
				if (mines[6] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of if
				else
				{
					buyMines(mines[6]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "8")
			{
				if (mines[7] > 11)
				{
					cout << "Your Mine is max level!" << endl;
				}//end of if
				else
				{
					buyMines(mines[7]);
				}//end of else
			}//end of else if
			else if (buymines_selection == "9")
			{
					if (mines[8] > 11)
					{
						cout << "Your Mine is max level!" << endl;
					}
					else
					{
						buyMines(mines[8]);
					}
			}//end of else if-
			else if (buymines_selection == "10")
			{
				if (mines[9] > 11)
				{
				cout << "Your Mine is max level!" << endl;
				}
				else
				{
				buyMines(mines[9]);
				}
			}//end of else 
			else if (buymines_selection == "11")
			{
			state_buymines = true;
			}//end of elseif
			else
			{
				cout << "Invalid selection" << endl;
			}//end of else
		}//end of while
		state_buymines = false;
	}//end of function
	void buyMines(int& inputted_integer)
	{
		upgrade_price = 0;
		int outputting_couter = 0;
		//This for loop is to calculate the price of the upgrade
		for (int upgrade_counter = 1; upgrade_counter < inputted_integer + 1; upgrade_counter++)
		{
			upgrade_price = upgrade_price + (4.1 * upgrade_counter);
			outputting_couter = upgrade_counter;
		}//end of for
		//This while is to ask the user if they are sure that they want to upgrade that specific mine
		while (state_confirmPurchase == false)
		{
			cout << "Are you sure that you want to upgrade Mine " << buymines_selection << " to level " << inputted_integer + 1 << " for " << upgrade_price << " gold?" << endl << "1) Yes" << endl << "2) No" << endl << "Selection: ";
			cin >> confirm_purchase;
			if (confirm_purchase == "1")
			{
				if (gold > upgrade_price)
				{
					mines[inputted_integer++];
					cout << "Mine " << outputting_couter << " is now level " << mines[inputted_integer] << endl;
					gold = gold - upgrade_price;
				}//end of if
				else if (gold < upgrade_price)
				{
					cout << "You do not have enough Gold!" << endl;
				}//end of else if
				state_confirmPurchase = true;
			}//end of if
			else if (confirm_purchase == "2")
			{
				state_confirmPurchase = true;
			}//end of else if 
			else
			{
				cout << "Invalid selection" << endl;
			}//end of else
		}//end of while
		state_confirmPurchase = false;
	}//end of function
	void calculateCollection()
	{
		collection_rate = 0;
		mine_capacity_MAX = 0;
		//exponential calculation for collection rate which differes from level of mine
		for (int collection_count = 0; collection_count < 10; collection_count++)
		{
			for (int indice = 0; indice < mines[collection_count]+1; indice++)
			{
				indice_number = 1.4 * indice;
				cout << indice_number << endl;
			}//end of for
			collection_rate = collection_rate + indice_number;
			indice_number = 0;
		}//end of for 
		//calcualtion for the maximum allowed to be collected per the different level of all the mines owned
		for (int mine_counter = 0; mine_counter < 10; mine_counter++)
		{
			for (int mine_capacity = 1; mine_capacity > mines[mine_counter]; mine_counter++)
			{
				mine_counter++;
				mine_capacity_MAX = mine_capacity_MAX + (4 * mine_counter);
				cout << mine_capacity_MAX << endl;
			}//end of for
		}//end of for
	}//end of function
	void collectMines()
	{
		calculateCollection();
		refreshGame();
		collection_time = current_time - saved_time;
		collection_amount = collection_time * collection_rate;
		if (collection_amount > mine_capacity_MAX)
		{
			collection_amount = mine_capacity_MAX;
			gold = gold + collection_amount;
			cout << "You collected " << collection_amount << " Gold!" << endl;
			collection_amount = 0;
		}
		else if (collection_amount == mine_capacity_MAX || collection_amount < mine_capacity_MAX)
		{
			gold = gold + collection_amount;
			cout << "You collected " << collection_amount << " Gold!" << endl;
			collection_amount = 0;
		}
		else
		{

		}
		collection_time = 0;
		saveGame();
	}//end of function
	//Storages
	void menuStorages()
	{
		while (state_storage == false)
		{
			cout << "Storage Menu - Please select an option" << setw(width) << "Gold: " << gold << endl << "1) Buy / Upgrade Storages" << endl << "2) Exit" << endl;
			for (int storagecount = 0; storagecount < 4; storagecount++)
			{
				cout << "Storage " << storagecount + 1 << " is Lv." << storage[storagecount] << endl;
			}
			cout << "Selection: ";
			cin >> storage_selection;
			if (storage_selection == "1")
			{
				buyStoragesMenu();
				state_storage = false;
			}
			else if (storage_selection == "2")
			{
				state_storage = true;
			}
			else
			{
				cout << "Invalid selection" << endl;
				state_storage = false;
			}
		}//end of while
		state_storage = false;
	}//end of function
	void buyStoragesMenu()
	{
		while (state_buymines == false)
		{
			cout << "What storage whould you like to Upgrade?" << endl << "Selection: ";
			cin >> buystorage_selection;
			if (buystorage_selection == "1")
			{
				
			}
			else if (buystorage_selection == "2")
			{

			}
			else if (buystorage_selection == "3")
			{

			}
			else if (buystorage_selection == "4")
			{

			}
			else
			{
				cout << "Invalid Selection" << endl;
			}
		}//end of while
	}//end of function 
	void buyStorages()
	{

	}//end of function
	int checkStorage()
	{
		int storage_avalible = 0;
		//storage_avalible = storage_capacity - gold;
		return storage_avalible;
	}//end of function
};//end of class
int main()
{
	bool state_program = false;
	string program_selctoion, week_day;
	Game Run;
	week_day = Run.findDay();
	cout << "Hello there! Today is " << week_day << endl;
	while (state_program == false)
	{
		Run.loadGame();
		cout << "Please select a menu to go to." << endl << "1) Option" << endl << "2) Mines" << endl << "3) Storage" << endl << "4) Betting Arenas" << endl << "5) Stock Market" << endl << "6) Exit" << endl << "Selection: ";
		cin >> program_selctoion;
		if (program_selctoion == "1")
		{
			Run.mainMenu();
		}//end of if
		else if (program_selctoion == "2")
		{
			Run.menuMines();
		}//end of else if
		else if (program_selctoion == "3")
		{
			Run.menuStorages();
		}//end of else if
		else if (program_selctoion == "4")
		{

		}//end of else if
		else if (program_selctoion == "5")
		{
			Run.findDay();
		}//end of else if
		else if (program_selctoion == "6")
		{
			state_program = true;
		}//end of else if
		else
		{
			cout << "Invalid seleciton!" << endl;
		}//end of else
	}//end of while
}//end of program
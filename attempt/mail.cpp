#include <iostream>
#include <deque>
#include <fstream>
#include <algorithm>
#include <string>
#include <windows.h>

#define DEBUG

using namespace std;

void make_whitespase(string& str, const bool whitespase);

class Thing
{
public:
	
	string GetName()
	{
		return Name;
	}
	void SetName(string Name)
	{
		this->Name = Name;
	}
	string GetAdress()
	{
		return Adress;
	}
	void SetAdress(string Adress)
	{
		this->Adress = Adress;
	}
	string GetType()
	{
		return Type;
	}
	void SetType(string Type)
	{
		this->Type = Type;
	}
	void Print()
	{
		string try_Name = this->GetName();
		string try_Adress = this->GetAdress();
		string try_Type = this->GetType();
		make_whitespase(try_Name, true);
		make_whitespase(try_Adress, true);
		make_whitespase(try_Type, true);
		std::cout << try_Name + '(' + try_Type + ')' + ' ' + '-' + ' ' + try_Adress << endl;
	}

private:
	string Name;
	string Adress;
	string Type;
};

bool find_maker(string const &qeschion, string const & str)
{
	const int size_qeschion = qeschion.size();
	const char *arr_qeschion = qeschion.c_str();
	const int size_str = str.size();
	const char* arr_str = str.c_str();

	for (int i = 0; i < size_str; i++)
	{
		if (arr_str[i] == arr_qeschion[0])
		{
			int j = 0;
			while(j<size_qeschion)
			{
				i++;
				j++;
				if (arr_str[i] != arr_qeschion[j])
				{
					break;
				}
				if (j == size_qeschion - 1)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void make_whitespase(string & str,const bool whitespase)
{
	const int size_array = str.size();
	const char* array = str.c_str();
	char *new_array = new char [size_array+1];
	for (int i = 0; i < size_array; i++)
	{
		new_array[i] = array[i];
		if (whitespase == false)
		{
			if (array[i] == ' ')
			{
				new_array[i] = '_';
			}
		}
		else
		{
			if (array[i] == '_')
			{
				new_array[i] = ' ';
			}
		}
	}
	new_array[size_array] = '\0';
	str = string(new_array);
}

ostream& operator << (ostream& os, Thing& object)
{
	os << object.GetName() << " " << object.GetAdress() << " " << object.GetType() <<'\n';
	return os;
}

istream& operator >> (istream& is, Thing& object)
{
	string try_Name;
	string try_Adress;
	string try_Type;
	is >> try_Name >> try_Adress >> try_Type;
	object.SetName(try_Name);
	object.SetAdress(try_Adress);
	object.SetType(try_Type);
	return is;
}

int main()
{
	//setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	Thing something;
	deque <Thing> book;
#ifdef DEBUG
	string path = "data.txt";
	fstream file;
	file.open(path,fstream::in|fstream::out);
	if (!file.is_open())
	{
		std::cout << "Проблема с файлом хранения данных!" << endl;
	}
	else
	{
		while(!file.eof())
		{
			file >> something;
			book.push_back(something);
		}
		std::cout << "База загружена." << endl;
	}
	file.close();
#endif // DEBUG

	while (true)
	{
		string ansver = " ";
		std::cout << "1 - искать" << endl << "2 - добавить новую вещь" << endl << "3 - показать всё" << endl << "4 - выйти" << endl;
		std::cin >> ansver;
		
		if (ansver == "1")
		{
			string reqest = " ";
			std::cout << "Ключевое слово:" << endl;
			std::cin >> reqest;
			make_whitespase(reqest, false);
			deque<Thing> result;
			copy_if(book.begin(), book.end(), back_inserter(result), [&reqest, &book](Thing& a)
				{
					return find_maker(reqest,a.GetAdress()) || find_maker(reqest,a.GetName()) || find_maker(reqest,a.GetType());
				});
			std::cout << "Найдено результатов: " << result.size() << endl;

			if (result.size() > 1)
			{
				char sorter = ' ';
				std::cout << "Отсортировать по:" << endl << "1 - Наименованию;" << endl << "2 - Расположению;" << endl << "3 - Тегу." << endl;
				std::cin >> sorter;
				switch (sorter)
				{
					case '1':
					{
						sort(result.begin(), result.end(), [&result](Thing a, Thing b)
							{
								return	a.GetName() < b.GetName();
							});
						break;
					}
					case '2':
					{
						sort(result.begin(), result.end(), [&result](Thing a, Thing b)
							{
								return	a.GetAdress() < b.GetAdress();
							});
						break;
					}
					case '3':
					{
						sort(result.begin(), result.end(), [&result](Thing a, Thing b)
							{
								return	a.GetType() < b.GetType();
							});
						break;
					}
					default:
					{
						cout << "Такого варианта нет." << endl;
					}
				}
			}
			std::cout << endl;
			for (auto ex : result)
			{
				ex.Print();
			}
			if (result.size() == 1)
			{
				char to_delete = ' ';
				std::cout << "5 - удалить запись" << endl << "Продолжить - любой символ" << endl;
				std::cin >> to_delete;
				switch (to_delete)
				{
				case '5':
				{
					Thing temp = result[0];
					book.erase(remove_if(book.begin(), book.end(), [&book, &temp](Thing a)
						{
							return a.GetAdress() == temp.GetAdress() && a.GetName() == temp.GetName() && a.GetType() == temp.GetType();
						}), book.end());
#ifdef DEBUG
					fstream file;
					file.open(path, fstream::in|fstream::out);
					if (!file.is_open())
					{
						std::cout << "Проблема с файлом хранения данных! Данные не сохранены." << endl;
					}
					else
					{
						for (auto ex : book)
						{
							file<<ex;
						}
						std::cout << "Записано." << endl << endl;
					}
					file.close();
#endif // DEBUG
				}
				default:
					break;
				}
			}

		}

		if (ansver == "2")
		{
			Thing th;
			string value = " ";
			std::cout << "Наименование вещи:" << endl;
			std::cin.seekg(std::cin.eof());
			getline(std::cin, value);
			make_whitespase(value, false);
			th.SetName(value);
			std::cout << "Расположение вещи:" << endl;
			std::cin.seekg(std::cin.eof());
			getline(std::cin, value);
			make_whitespase(value, false);
			th.SetAdress(value);
			std::cout << "Тег:" << endl;
			std::cin.seekg(std::cin.eof());
			getline(std::cin, value);
			make_whitespase(value, false);
			th.SetType(value);
			book.push_back(th);
#ifdef DEBUG
			fstream file;
			file.open(path,fstream::in|fstream::out);
			if (!file.is_open())
			{
				std::cout << "Проблема с файлом хранения данных! Данные не сохранены." << endl;
			}
			else
			{
				for (auto ex : book)
				{
					file << ex;
				}
				std::cout << "Записано." << endl << endl;
			}
			file.close();
#endif // DEBUG
		}
		else if (ansver == "3")
		{
			for (auto ex : book)
			{
				ex.Print();
			}
		}
		else if (ansver == "4")
		{
			break;
		}
		else
		{
			std::cout << endl;
		}
		
	}
	SetConsoleOutputCP(866);
	SetConsoleCP(866);

	return 0;
}

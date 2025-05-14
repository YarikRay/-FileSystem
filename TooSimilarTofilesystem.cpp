#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <sstream>
#include "windows.h"

using namespace std;

/*/

class RecursiveIterator
{
public:
	wstring path_file;
	RecursiveIterator() : path_file(L"C:\\Users\\yaros\\Desktop\\Библиотека filesystem\\Код проекта\\TooSimilarTofilesystem\\TestDirectory\\*") {}
	WIN32_FIND_DATA ffd;
	void FileEnumeration()
	{
		HANDLE hFind = FindFirstFile(path_file.c_str(), &ffd);

		do
		{
			/*if (wcscmp(ffd.cFileName, L".") == 0 or wcscmp(ffd.cFileName, L"..") == 0)
			{
				continue;
			}
			wcout << ffd.cFileName << endl;
		
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Пропускаем родительскую и текущую директории
				if (wcscmp(ffd.cFileName, L".") == 0 or wcscmp(ffd.cFileName, L"..") == 0)
				{
					continue;
				}
				wcout << ffd.cFileName << endl;
				wstring path_file_copy = path_file;
				path_file = path_file + L"\\" + ffd.cFileName + L"\\*"; // обновляем путь

			}
			else {
				if (wcscmp(ffd.cFileName, L".") == 0 or wcscmp(ffd.cFileName, L"..") == 0)
				{
					continue;
				}
				wcout << ffd.cFileName << endl;
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}
};*/

#pragma pack(push,1)

struct file_info
{
	bool directory; // проверка того, что это директория
	wstring full_path; // полный путь
	wstring name; // имя файла
	wstring d_name; // имя директории
	int size; // размер
	wstring extension; // расширение
};

template<typename T>
class Iterator
{
private:
	T* current_position;
	T* end;
public:
	// iterator_traits
	using iterator_category = forward_iterator_tag; // тип итератора. Будем предпологать, что двигаться мо контейнеру можно только в одну сторону и только вперед.
	// Всего в STL существует 5 типов итераторов. Нам подходит forwarditerator, так как именно он позволяет двигаться в соответствующем направлении
	using value_type = T; // тип значений, по которому "бежит" итератор
	using difference_type = ptrdiff_t;  // позволяет хранить расстояние между 2-мя итераторами. ptrdiff_t - это знаковый целочисленный тип, способный хранить расстояние
	// между 2-мя указателми
	using pointer = T*; // определяет тип указателя, на который указывает итератор
	using reference = T&; // определяет тип ссылки, возвращаемый при разыменовании
	// Конструктор и деструктор
	Iterator(T* start_pos, T* end_pos) : current_position(start_pos), end(end_pos) {}
		//~Iterator();

	Iterator& operator++()
	{
		if (current_position != end)
		{
			++current_position; // перемещаем указатель на следующий элемент
		}
		return *this; // возвращает обновленный current_position
	}; // переход к следующему объекту

	reference operator*() 
	{
		return *current_position;
	}; // указывает на итератор для конкретного объекта

	bool operator!=(const Iterator& ) const // вовращает ссылку на итератор для объекта
	{
		return this->current_position != end; 
	};
		
};

template<typename T>
class Directory_It 
{
private:
	WIN32_FIND_DATA ffd;
	
	file_info file;

public:
	
	// Создаем структурный вектор
	vector<T> info;
	
	
	// Конструктор со списком инициализации
	//Directory_It(wstring path_en) : path(path_en) {}

	// Метод, реализующий логику перечисления файлов и каталогов
	//WIN32_FIND_DATA ffd;
	Directory_It(wstring path_ent) { enumeration_method(path_ent); };
	
	// Рекурсивный подсчет размера директорий
	int calc_size(wstring path_ent)
	{
		int size = 0;
		wstring present_path = path_ent + L"\\*";
		HANDLE hFind = FindFirstFileW(present_path.c_str(), &ffd);
		do
		{
			if (wcscmp(ffd.cFileName, L".") == 0 or wcscmp(ffd.cFileName, L"..") == 0)
			{
				continue;
			}
			wstring temp_path = path_ent + L"\\" + ffd.cFileName;
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				size+=calc_size(temp_path);
			}
			else
			{
				// Определение размера
				//wcout << temp_path << endl;
				ifstream in(temp_path, ios::binary);
				if (in.is_open())
				{
					in.seekg(0, ios::end);

					size += in.tellg();
					in.seekg(0);
				}
				else
				{
					cout << "File's not open" << endl;
				}
			}
			//cout << size << endl;
			
		}while (FindNextFile(hFind, &ffd));
		
		return size;
	}

	// Общая информация о файлах
	void enumeration_method(wstring path_ent)
	{
		//info.reserve(1000);
		wstring present_path =  path_ent + L"\\*";
		
		HANDLE hFind = FindFirstFileW(present_path.c_str(), &ffd);
	
		do
		{
			
			if (wcscmp(ffd.cFileName, L".") == 0 or wcscmp(ffd.cFileName, L"..") == 0)
			{
				continue;
			}
			
			
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
			{
				
				
				wstring temp_path = path_ent + L"\\" + ffd.cFileName;
				file.full_path = temp_path;
				file.name = ffd.cFileName;
				file.directory = true;
				file.extension = L"None";
				file.size = calc_size(temp_path);
				info.push_back(file);
				enumeration_method(temp_path); // temp_path
				
			}
			else
			{
				
				file.name = ffd.cFileName; // имя файла
				file.full_path = path_ent +L"\\" + ffd.cFileName; // полный путь
				file.directory = false;
				// Определение размера
				ifstream in(file.full_path, ios::binary);
				if (in.is_open())
				{
					in.seekg(0, ios::end);

					file.size = in.tellg();
					in.seekg(0);
				}
				else
				{
					cout << "File's not open" << endl;
				}
				

				// Определение расширения
				size_t pos = file.name.find_last_of('.'); // делаем срез строки, начиная с точки и до конца 
				file.extension = file.name.substr(pos); 
				
				info.push_back(file);
			}
			
		} while (FindNextFile(hFind, &ffd));
		
		FindClose(hFind);
		
	}
	// Метод begin
	Iterator<T> begin() { return Iterator<T>(info.data(), info.data() + info.size()); }
	// Метод end
	Iterator<T> end() { return Iterator<T>(info.data() + info.size(), info.data() + info.size()); }
	
	


};
#pragma pack(pop)
int main()
{
	setlocale(LC_ALL, "RU");
	Directory_It<file_info> it(L"C:\\Users\\yaros\\Desktop\\Библиотека filesystem\\Код проекта\\TooSimilarTofilesystem\\TestDirectory");
	

	HWND consoleWindow = GetConsoleWindow();
	ShowWindow(consoleWindow, SW_MAXIMIZE);  // Развернуть на весь экран

	
	stringstream s;
	cout << left << setw(24) << "File Name" << "Size" << "     " << "Extension" << "         " << "Directory?" << "                                                      " 
		<< "Full Path" <<  endl;
	for (Iterator<file_info> i = it.begin(); i != it.end(); ++i)
	{
		file_info& it = *i;
		wcout << left <<  setw(25) << it.name << left << setw(10) << left << setw(10) << it.size << left << setw(20) << it.extension << left << setw(15) <<
			it.directory << " " << it.full_path << endl;
		
		
	}



}
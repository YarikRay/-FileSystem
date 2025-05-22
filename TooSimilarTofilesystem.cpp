#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stack>
#include <windows.h>

using namespace std;

#pragma pack(push,1)

struct file_info {
    bool directory;
    wstring full_path;
    wstring name;
    wstring d_name;
    int size;
    wstring extension;
};

template<typename T>
class Iterator {
private:
    T* current_position;
    T* end;
public:
    using iterator_category = forward_iterator_tag; // тип итератора
    using value_type = T; // тип итерируемого значения
    using difference_type = ptrdiff_t; // разность между 2-мя итераторами
    using pointer = T*; // тип указателя
    using reference = T&; // тип ссылки

    Iterator(T* start_pos, T* end_pos) : current_position(start_pos), end(end_pos) {}

    Iterator& operator++() {
        if (current_position != end) {
            ++current_position; // сдвигаем итератор на следующий элемент
        }
        return *this; // возвращаем итератор
    }

    reference operator*() {
        return *current_position; // возвращаем сам элемент через разыменовывание
    }

    bool operator!=(const Iterator&) const {
        return this->current_position != end; // проверяем не достиг ли итератор конца
    }
};

template<typename T>
class Directory_It {
private:
    WIN32_FIND_DATA ffd;
    file_info file;

public:
    vector<T> info;

    Directory_It(wstring path_ent) { enumeration_method(path_ent); } // констурктор. Вызываем обход файлов при создании объекта класса Directory_It

    // Подсчет общего объема директорий
    int calc_size(wstring path_ent) {
        int size = 0;
        //vector<wstring> dirs_to_process = { path_ent };
        deque<wstring> dirs_to_process = { path_ent }; // создание контейнера

        while (!dirs_to_process.empty()) {
            wstring current_path = dirs_to_process.front(); // обращаемся к первому элементу
            dirs_to_process.pop_front(); // удаляем самый первый элемент

            wstring search_path = current_path + L"\\*"; // добавляем *, где * - это шаблон, по которому будет производиться поиск файлов, т.е будут учитываться все файлы
            HANDLE hFind = FindFirstFileW(search_path.c_str(), &ffd); // находим первый файл

            // Обработка ошибок. К примеру, к файлам доступа нет.
            if (hFind == INVALID_HANDLE_VALUE) {
                DWORD err = GetLastError();
                if (err != ERROR_FILE_NOT_FOUND and err != ERROR_NO_MORE_FILES) {
                    wcout << L"Ошибка доступа: " << search_path << endl;
                }
                continue; // продолжаем рабту, несмотря на ошибку
            }


            do {
                if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0) {
                    continue; // пропускаем текущую и родительскую директории
                }

                wstring full_path = current_path + L"\\" + ffd.cFileName; // обновляем путь

                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    dirs_to_process.push_back(full_path); // если директория, то добавляем новый путь в контейнер deque
                }
                else {
                    // Считаем размер
                    ifstream in(full_path, ios::binary);
                    if (in.is_open()) {
                        in.seekg(0, ios::end);
                        size += in.tellg();
                        in.close();
                    }
                }
            } while (FindNextFileW(hFind, &ffd)); // ищем новый файл

            FindClose(hFind);
        }
        return size; // возвращаем размер
    }

    // Аналогично calc_size-функции
    void enumeration_method(wstring path_ent) {
        //vector<wstring> dirs_to_process = { path_ent };
        deque<wstring> dirs_to_process = { path_ent };
        while (!dirs_to_process.empty()) {
            wstring current_path = dirs_to_process.front();
            dirs_to_process.pop_front();

            wstring search_path = current_path + L"\\*";
            HANDLE hFind = FindFirstFileW(search_path.c_str(), &ffd);

            if (hFind == INVALID_HANDLE_VALUE) {
                DWORD err = GetLastError();
                if (err != ERROR_FILE_NOT_FOUND and err != ERROR_NO_MORE_FILES) {
                    wcout << L"Ошибка доступа: " << search_path << endl;
                }
                continue;
            };

            do {
                if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0) {
                    continue;
                }

                wstring full_path = current_path + L"\\" + ffd.cFileName;    
                file_info file;

                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    // Если объект - директория, то записываем все резльтаты для диреткории в структурную переменную file
                    file.full_path = full_path;
                    file.name = ffd.cFileName;
                    file.directory = true;
                    file.extension = L"None";
                    file.size = calc_size(full_path);
                    info.push_back(file);
                    dirs_to_process.push_back(full_path);
                }
                else {
                    // Если объект - файл, то записываем все результаты для файла в структурную переменную file
                    file.name = ffd.cFileName;
                    file.full_path = full_path;
                    file.directory = false;

                    ifstream in(file.full_path, ios::binary);
                    if (in.is_open()) {
                        in.seekg(0, ios::end);
                        file.size = in.tellg();
                        in.close();
                    }

                    size_t pos = file.name.find_last_of('.'); // ищем индекс точки
                    if (pos != string::npos) // проверям, найдена ли точка. Если точка найдена, то делается срез символов после точки - расширение
                        // Если не найдена, то инструкция npos запишет в pos максимальный размер числа, которй может принять size_t и запишет в file.extension None
                    {
                        file.extension = file.name.substr(pos);
                    }
                    else
                    {
                        file.extension = L"None";
                    }
                    
                    info.push_back(file);
                }
            } while (FindNextFileW(hFind, &ffd));

            FindClose(hFind);
        }
    }
    // Добавляем поддержку begin и end
    Iterator<T> begin() { return Iterator<T>(info.data(), info.data() + info.size()); } // передаем указатели на начало обхода и конец
    Iterator<T> end() { return Iterator<T>(info.data() + info.size(), info.data() + info.size()); } // передаем указаетль на конец обхода
};

#pragma pack(pop)

int main() {
    setlocale(LC_ALL, "RU");

    // Для тестирования лучше использовать конкретную папку, а не весь диск C:
    Directory_It<file_info> it(L"C:\\Users\\yaros\\Desktop\\Библиотека filesystem\\Код проекта\\TooSimilarTofilesystem\\TestDirectory");  // Пример: обход только папки Windows

    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);

    cout << left << setw(24) << "File Name" << "Size" << "     " << "Extension" << "         "
        << "Directory?" << "                                                      " << "Full Path" << endl;


    for (Iterator<file_info> i = it.begin(); i != it.end(); ++i) {
        file_info& item = *i;

        wcout << left << setw(25) << item.name << left << setw(10) << item.size
            << left << setw(20) << item.extension << left << setw(15) << item.directory
            << " " << item.full_path << endl;
    }

    return 0;
}
# Инструкция
## 1) Описание данного проекта.
   Данная программа являестя схожей с библиотекой C++ std::filesystem. Код состоит из 2-ух классов, структуры и функции main.
   ### Перечислим дополнительные библиотеки, которые были использованы во время разработки:
         1. <string> - библиотка использовалась для работы со строками. Необходимо было выделить расширение файла от имени файла.
         2. <vector> - библиотка использовалась для работы с векторами. В данном коде было необходимо создать структурный вектор.
         3. <fsream> - библиотка использовалась для чтения файла. В данной программе надо было открывать файл для считывания его размера.
         4. <iomanip> - библиотека использовалась для корректного выравнивания при выовде в консоль при помощи setw.
         5. <windows.h> - основная библиотека данного проекта. С помощью функций WinApi находились файлы в директориях.
	 6. <deque> - двусторонняя очередь. Контейнер, в который складываются пути, по которым осуществляется обход.
   ### Опишем назначение структуры и классов:
   **1. Структура file_info:**
   ###            
   ###               Состоит из 6 переменных:
                          1) directory - имеет тип bool. Определяет являестя ли очередной объект директорией или простым файлом (1 - директория, 0 - файл).
                          2) full_path - имеет тип wstring. Содержит полный путь до файла/директории.
                          3) name - имеет тип wstring. Содержит имя следующего файла или следующей директории.
                          4) d_name - лишняя переменная. Не используется дальше.
                          5) size - имеет тип int. Сохраняет размер файла/директории.
                          6) extension - имеет тип wstring. Хранит в себе расширение очередного файла.
   **2. Класс Iterator:**
   
   Является шаблонным классом. Делится на 2 раздела: private и public. Используется iterator_traits для соответствия интерфейсу стандартного итератора STL.
   ### Используется 2 указателя типа T (подставляемый тип данных):
                          1) T* current_position.
                          2) T* end;
   Указатель current_position определяет текущее положение итератора, указатель end определяет конец контейнера. Данные указатели записаны в разделе private.
   Класс используется для итерирования по контейнеру. В данном случае в роли контейнера используется структурный вектор.
   
   **3. Класс Directory_It**
   
   Является шаблонным классом. Делится на 2 раздела: private и public. Засчет данного класса выполнятеся основная логика работы программы. Выполняется поиск файлов в директориях,
   определяется является ли объект директорией/файлом, считает размер и т.д. Содержит в себе вектор типа T, где T подставляемы тип данных. Далее подставляться будет структура file_info.
   ###                Используются следующие переменные:
                          1) file - тип file_info. Структурная переменная, в которую записываются результаты.
                          2) ffd - тип WIN32_FIND_DATA. Структурная переменная, использованная для работы с функциями WinApi. К примеру, ffd.cFileName содержит имя очередного объекта.
                          3) size - тип int. Необходима для записывания общего размера директории.
                          4) present_path - тип wstring. Записывает текущий путь и добавляе символ "*" для того, чтобы рассматривались все объекты, используется в качестве шаблона для поиска.
                          5) temp_path - тип wstring. Записывает текущий путь + очередное найденное имя объекта. Используется для рекурсивного обхода директории.
                          6) hFind - тип HANDLE. Используется для работы с функциями WinAPi. 
                          7) pos - тип size_t. Сохраняет индекс последнего символа перед расширением.
   ###               Содержит следующие методы:
                          1) calc_size - функция типа int. Возвращает размер файла/диретории. Рекурсиво подсчитывает размер диреторий.
                          2) enumeration_method - функция типа void. Реализует логику рекурсчивного поиска файлов. Определяет размер файла, определяет расширение. Производит запись в вектор
                          3) begin - функция типа Iterator. Передает указатель на начало обхода контейнера.
                          4) end - функция типа Iterator. Передает указатель на конец обхода контейнера.
## 2) Логика работы итератора
   iterator_traits явялеется основой работы итератора. Позволяет привести к соответствию интерфейс разработанного итератора к нтерфейсу стандартного итератора STL. 
   Шаблонный класс в стандартной библиотеке C++.
   ### Описание iterator_traits
                          1) iterator_category определяет тип итератора. Типы итераторов в C++: input iterator (поддерживает чтение элементов),
                          output iterator (поддерживает запись элементов), forward_iterator (поддерживает запись и чтение элементов,
                          однонапрвленный обход элементов),
                          bidirectional iterator (поддерживает и запись, и чтение, а также возможность обходить контейнер в обратном направлении (декрементировать)),
                          random access iterator (объедняет в себе все свойства перечисленных итераторов, а также поддерживает арифметику итераторов)
                          2) value_type определяет тип элемента, на который указывает итератор.
                          3) difference_type определяет расстояние между итераторами. Под расстоянием между итераторами понимается количество элементов между итераторами, указывающими на один и тот же контейнер.
                          4) pointer определяет тип указателя на элемент. С помощью указателя можно получить доступ к значеним контейнера. 
                          5) reference определяет тип ссылки на элемент. С помощью ссылки можно вернуть элемент, на который указывает итератор.
   В данном классе iterator_category принимает тип forward_iterator_tag, то есть можно двигаться только в одном направлении по контейнеру, value_type принимает значение T, так как класс Iterator является шаблонным, где T - подставляемый тип, 
   difference_type принимает значение ptrdiff_t, где ptrdiff_t - это знаковый целочисленный тип, предназначеныый для хранения разницы между 2-мя указателями (т.е., к примеру, один указатель ставится на начало обхода, другой - на конец, разница между
   концом обхода и началом записывается в difference_type), pointer прнимает тип T*, reference принимает тип T&. 
   ### Описание логики итератора
   Конструктор Iteator является параметризованным. В параметах передаются указатели типа T на начало обхода контейнера и на конец обхода контейнера. В списке инициализации указатель current_position приобретает значение start_pos,
   указатель end приобретает значение end_pos. Таким образом, теперь итератор знает, где контейнер начинается, а где он заканчивается.
   В каждом итераторе STL должны определять следующие методы: ++, *, !=.
   ### Опишем работу этих операторов последовательно:
                          1) ++ - оператор, позволяющий передвигаться по контейнеру последовательно. Если итератор не дашел до конца контейнера, то необходимо увеличивать current_position. Сам оператор определяется, как
                          Iterator& operator++(), тем самым обеспечивая возвращание самой позиции итератора, а не ее копии. Увеличение позиции указателя на элемент происходит с помощью ++current_position, возвращение итератора - return *this, где this - указатель
                          на обновленную позицию. С помощью оператора *, указатель this разыменовывается, возвращая позицию итератора на текущий элемент.
                          2) Оператор разыменовывания позволяет вернуть значение элемента, на который указывает итератор. Возвращается элемент с помощью return *current_position. Данный оператор определяется через reference для совместимости с STL.
                          3) != - оператор сравнения. Дает проверку на то, достигла ли current_position конца обхода контейнера. Оператор возвращает либо True, либо False. Определяется, как bool operator!=(const Iterator& ) const, где const Iterator& - это
                          ссылка на итератор для элемента. Возвращение осуществляется через условие this->current_position != end. this->current_position позволяет сравнить позицию итератора для текущего элемента с позицией итератора для конца контейнера.
   ### Описание работы класса Directory_It
   Данный класс является шаблонным с целью возможности работать с любыми типами. Работа класса начинается с создания параметризованного конструктора, который в качестве аргумента принимает путь до директории, которую необходимо рекурсивно 
   обойти. В реализации конструктора будем вызывать функцию enumeration_method. Т.е., когда будет создаваться объект класса Directory_It, будет вызваться функция enumeration_method с аргументом пути до директории.
   ### Описание функции calc_size
   С целью подсчета размера вложенных директорий, реализуем функцию calc_size, которая на вход принимает путь до директории. Объявим контейнер deque типа wstring. Положим в него путь до директории. Запускаем цикл while с условием проверки пустоты директория. 
   Если директория не окажется пустой, то в переменную типа wstring current_path с попомщью оператора front запишется путь до целевой директории. Важно заметить, что оператор front обратится к элементу, который является самым первым. К примеру, 
   контейнер состоит из следующих элементов: 1, 2, 3, 4. Инструкция front обратится именно к 1.
   Далее этот элемент удалаяется из контейнера с помощью операции pop_front. Далее в переменную search_path добавляется переменная current_path + шаблон. Выглядит это так: wstring search_path = current_path + L"\\*"; 
   Затем найдем первый файл в директории с помощью HANDLE hFind = FindFirstFileW(present_path.c_str(), &ffd); Также добавим обработку ошибки, т.е., к примеру, в записях системных дисков лежат файлы или директории, к которым нет доступа. Обраотка такой ошибки 
   напишет об этом и укажет путь до конкретного файла, который является недоступным. 
   Запускаем цикл do {} while();, в котором с помощью условия if (wcscmp(ffd.cFileName, L".") == 0 or wcscmp(ffd.cFileName, L"..") == 0) пропускаем текущую и родительскую директорию. Если такие находятся, просто продолжаем работу программы.
   В переменную full_path добавляем первый найденный файл, тем самым обновляя путь. Создаем структурную переменную file.  С помощью условия if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) определяем является ли вновь найденный файл директорией. Если да, то
   в конец конейнера кладем full_path, таким образом, реализуем вход в поддиректорию. Если найденный файл не директория, то считаем размер отдельного файла и добавляем этот рамер в переменную size, тем
   самым, считая общий объем поддиректории.
   Далее в цикле while пишем FindNextFile(hFind, &ffd) для поиска следующих файлов. 
   После окончания работы алгоритма, возвращаем size.
   ### Описание функции enumeration_method
   Функция enumeration_method реализована схожим образом. Также осуществляется итеративный обход директорий с записей полученных значений в структурный вектор. 
   С помощью условия if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) определяется директория ли вновь попавшийся объект или файл. Если директория, то записываются в переменные структуры file_info следующие значения:
   file.full_path = temp_path;
   file.name = ffd.cFileName;
   file.directory = true;
   file.extension = L"None";
   file.size = calc_size(temp_path);
   Далее эта переменная file аписывается в вектор с помощью info.push_back(file). После этого осуществляется вход в поддиректорию путем добавления переменной full_path в контейнер. Добавляется так: dirs_to_process.push_back(full_path);.
   Если же объект является файлом, то также в file.name кладется ffd.cFileName, в file.full_path сохраняется переменная full_path, которая содержит следующие: current_path + L"\\" + ffd.cFileName. Переменная file.directory становится равной False, так как перед 
   нами обычный файл.
   Далее с помощью потока ifstream определяем размер файла и сохраняем его в file.size. Для определения расширения позицианируемся в точку навания файла. К примеру, файл test.txt, с помощью команды file.name.find_last_of('.');
   находим индекс точки после которого начинается расширение файла. Записываем этот индекс в переменную pos. Перед этим осуществляем небольшую проверку того, что точка в имени файла вообще найдена. Если она не найдена, а срез произошел, то вызовется исключение
   out_of_range. Условие работает следующим образом, if (pos != string::npos), где pos индекс символа точки в имени файла. Если точка не найдена, возвращается npos, в котором будет лежать максимальное число, которое может принять size_t, а также file.extension =  
   L"None". Если же точка найдена, то в переменную file.extension записываем срез строки, в которой находится имя файла, начиная с точки до конца строки.
   Получаем расширение. 
   Итоговый код для нахождения расширения:
   size_t pos = file.name.find_last_of('.');
   if (pos != string::npos)
   {
       file.extension = file.name.substr(pos);
   }
   else
   {
       file.extension = L"None";
   }
   Далее записываем структурную переменную file в структурный вектор. В конце завершаем сессиию работы с файлами, используя функцию WinApi FindClose(hFind).
   ### Описание методов begin и end
   Метод begin имеет тип Iterator. Возвращает итератор на начало и на конец контейнера. Указатель на начало передается так - info.begin, указатель на конец передается с помощью info.data() + info.size()
   Метод end имеет тип Iterator. Возвращает итератор на конец контейнера с помощью info.data() + info.size().
   Полная реализация методов begin и end:
   Iterator<T> begin() { return Iterator<T>(info.data(), info.data() + info.size()); }
   Iterator<T> end() { return Iterator<T>(info.data() + info.size(), info.data() + info.size()); }
   Благодаря такой реализации методов begin и end появляется поддержка begin и end в дальнейшем классе for.
   Такая передача аргументов в Iterator возможна из-за устройства конструктора класса Iterator.
## Реализация функции main
   В функции main создается объект класса Directory_It типа <file_info>. В аргумент передается путь до директории. 
   Далее идет строчки кода, которые просто необходимы для вывода информации в консоль
   HWND consoleWindow = GetConsoleWindow();
   ShowWindow(consoleWindow, SW_MAXIMIZE);
   cout << left << setw(24) << "File Name" << "Size" << "     " << "Extension" << "         " << "Directory?" << "                                                      "  << "Full Path" <<  endl
   Создаем цикл for, куда передаем итератор типа file_info и инициализируем его указателем на начало контейнера, затем, импользуя оператор !=, сравниваем с концом контейнера ,потом с помощью оператор ++ начинаем обходить контейнер.
   Благодаря тому, что i является объектом класса Iterator, возможно использовать все операторы, которые были определены в классе Iterator.
   Реализация цикла for
   for (Iterator<file_info> i = it.begin(); i != it.end(); ++i)
   {
   	file_info& it = *i; // Оператор *, который исользуется для возвращения самого значения контейнера, на которое указывает итератор
   }
   
   В теле цикла осуществляем вывод значений.
## Почему deque?
   std::deque - контейнер STL C++. Позволяет обращаться к элементам как спереди контейнера, так и сзади.
   В данной программе это дает правильную, последовательную обраотку поддиректорий.
   К примеру, есть директория "C:/Users/Desktop", на рабочем столе лежат 2 поддиректории: "Windows", "Linux".
   Понятно, что при первом обходе первым объектом найдется Windows, также она определится, как директория. 
   Если это директория, то в контейнер dirs_to_progress запишется "C:/Users/Desktop/Windows", далее следующим шагом, функция FindNextFile найдет диреткорию Linux, котораяя также определится, как директория и запишется в dirs_to_process.
   Так как больше ничего в директии Desktop не найшлось, цикл do {} while() завершает свою работу. Но при этом цикл while - внешний общий цикл работу завершить не может, так как контейнер dirs_to_process не пуст.
   Следуя по алгоритму, в переменную current_path при заходе в цикл, запишется самый первый путь, лежащий в контейнере типа deque, это будет "C:/Users/Desktop/Windows". Самый первый элемент берем с помощью front. В конец онтейнера
   записываем при помощи push_back.
   Таким образом, мы зашли внутрь директории. Самое главное, что поддиректории будут обрабатываться
   по порядку, тем самым обеспечивая работоспособность данной программы.

				
   
    
                          
                         










   

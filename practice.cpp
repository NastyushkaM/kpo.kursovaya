#include <windows.h> // Для работы с оконными функциями в Windows
#include <cmath> // Для математических функций - pow, sin
#include <wchar.h> // Добавляем заголовочный файл для функции swprintf_s

// Прототипы функций обработки сообщений
LRESULT CALLBACK GraphSelectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GraphDisplayWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance; // Экземпляр приложения
HWND g_hCurrentGraphWindow = NULL; // Указатель на текущее окно графика

// Перечисление различных графиков
enum GraphType { LINEAR, SINUS, PARABOLA, PARABOLA2};

// Класс окна для выбора графика
class GraphSelectWindow
{
public:
    // Конструктор принимает экземпляр приложения
    GraphSelectWindow(HINSTANCE hInstance) : hInstance(hInstance) {}

    // Метод для создания окна
    void Create()
    {
        const wchar_t CLASS_NAME[] = L"Класс окна выбора графика";

        WNDCLASS wc = { };

        wc.lpfnWndProc = GraphSelectWindowProc; // Функция обработки сообщений для окна 
        wc.hInstance = hInstance; // Экземпляр приложения
        wc.lpszClassName = CLASS_NAME; // Имя класса

        // Регистрируем класс окна
        RegisterClass(&wc);

        // Создаем окно
        hwnd = CreateWindowEx(
            0, // Отсутствие дополнительных стилей
            CLASS_NAME, // Имя класса
            L"Выбор графика", // Заголовок окна
            WS_OVERLAPPEDWINDOW, // Стандартный стиль окна
            50, // Координата x
            50, // Координата y
            400, // Ширина окна
            400, // Высота окна
            NULL, // Окно не имеет родительского окна
            NULL, // Отсутствие меню
            hInstance, // Дескриптор экземпляра приложения, которому принадлежит окно
            NULL // Пользовательские данные отсутствуют 
        );
        
        ShowWindow(hwnd, SW_SHOW); // Показываем окно
    }
    // Метод для получения дескриптора окна
    HWND GetHandle() const { return hwnd; }

private:
    HINSTANCE hInstance; // Экземпляр приложения, необходимый для создания окна
    HWND hwnd; // Дескриптор окна - используется для идентификации созданного окна
};

// Класс окна для отображения графика
class GraphDisplayWindow
{
public:
    // Конструктор принимает экземпляр приложения, имя графика и тип графика
    GraphDisplayWindow(HINSTANCE hInstance, LPCWSTR graphName, GraphType type)
        : hInstance(hInstance), graphName(graphName), graphType(type) {}

    // Метод для создания окна
    void Create()
    {
        const wchar_t CLASS_NAME[] = L"Класс окна отображения графика";

        WNDCLASS wc = { };

       
        wc.lpfnWndProc = GraphDisplayWindowProc; // Функция обработки сообщений для этого окна
        wc.hInstance = hInstance; // Экземпляр приложения
        wc.lpszClassName = CLASS_NAME; // Имя класса

        // Регистрируем класс окна
        RegisterClass(&wc);

        // Создаем окно
        hwnd = CreateWindowEx(
            0, // Отсутствие дополнительных стилей
            CLASS_NAME, // Имя класса
            graphName, // Заголовок окна - меняется в зависимости от графика
            WS_OVERLAPPEDWINDOW, // Стандартный стиль окна
            500, // Координата x
            50, // Координата y
            800, // Ширина окна
            600, // Высота окна
            NULL, // Окно не имеет родительского окна
            NULL, // Отсутствие меню
            hInstance, // Дескриптор экземпляра приложения, которому принадлежит окно
            this // Передаем указатель на объект окна
        );

        ShowWindow(hwnd, SW_SHOW);// Показываем окно
    }
    // Метод для получения дескриптора окна
    HWND GetHandle() const { return hwnd; }
    // Возвращает тип графика, который был передан при создании объекта
    GraphType GetType() const { return graphType; }

private:
    HINSTANCE hInstance;  // Экземпляр приложения
    LPCWSTR graphName;  // Имя графика
    HWND hwnd;  // Дескриптор окна
    GraphType graphType;  // Тип графика
};

// Главная функция приложения - точка входа для приложения Windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Сохраняем экземпляр приложения в глобальной переменной
    g_hInstance = hInstance;

    // Создаем окно для выбора графика
    GraphSelectWindow graphSelectWindow(hInstance);
    graphSelectWindow.Create(); // Вызов метода Create() объекта graphSelectWindow, который создает и отображает окно для выбора графика

    // Запуск цикла сообщений
    MSG msg = { }; // Переменная для хранения сообщений
    while (GetMessage(&msg, NULL, 0, 0)) //Пока не будет получено сообщение о завершении приложения
        DispatchMessage(&msg); // Функция отправляет сообщение соответствующей оконной процедуре для дальнейшей обработки
   
    return 0;
}

// Обработчик сообщений окна выбора графика
LRESULT CALLBACK GraphSelectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    // Кейс обрабатывает сообщение о создании окна
    case WM_CREATE:
    {
        // Создание кнопок выбора графика
        CreateWindow(L"BUTTON", L"График линейной функции", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 50, 250, 50, hwnd, (HMENU)1, NULL, NULL);

        CreateWindow(L"BUTTON", L"График синуса", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 120, 250, 50, hwnd, (HMENU)2, NULL, NULL);

        CreateWindow(L"BUTTON", L"График квадратичной функции", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 190, 250, 50, hwnd, (HMENU)3, NULL, NULL);

        CreateWindow(L"BUTTON", L"График степенной функции", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 260, 250, 50, hwnd, (HMENU)4, NULL, NULL);
    }
    break;
    // Кейс обрабатывает сообщения о командах, такие как нажатие на кнопку в окне
    case WM_COMMAND:
        switch (LOWORD(wParam)) // Для определения кнопки, которую нажали
        {
        case 1: // Выбор прямой
        {
            if (g_hCurrentGraphWindow != NULL) // Проверка на существования окна графика 
            {
                DestroyWindow(g_hCurrentGraphWindow); // Уничтожение окна
                g_hCurrentGraphWindow = NULL;
            }
            // Создаем и отображаем окно графика линейной функции
            GraphDisplayWindow* graphDisplayWindow = new GraphDisplayWindow(g_hInstance, L"График линейной функции - прямая", LINEAR); // Создание нового экземпляра класса 
            graphDisplayWindow->Create(); // Создание и отображение
            g_hCurrentGraphWindow = graphDisplayWindow->GetHandle(); // Сохранения для дальнейшего управления
        }
        break;

        case 2: // Выбор синусоиды
        {
            if (g_hCurrentGraphWindow != NULL) 
            {
                DestroyWindow(g_hCurrentGraphWindow);
                g_hCurrentGraphWindow = NULL;
            }
            // Создаем и отображаем окно графика синуса
            GraphDisplayWindow* graphDisplayWindow = new GraphDisplayWindow(g_hInstance, L"График синуса - синусоида", SINUS); 
            graphDisplayWindow->Create(); 
            g_hCurrentGraphWindow = graphDisplayWindow->GetHandle();
        }
        break;

        case 3: // Выбор параболы
        {
            if (g_hCurrentGraphWindow != NULL) {
                DestroyWindow(g_hCurrentGraphWindow);
                g_hCurrentGraphWindow = NULL;
            }
            // Создаем и отображаем окно графика параболы
            GraphDisplayWindow* graphDisplayWindow = new GraphDisplayWindow(g_hInstance, L"График квадратичной функции - квадратная парабола", PARABOLA);
            graphDisplayWindow->Create();
            g_hCurrentGraphWindow = graphDisplayWindow->GetHandle();
        }
        break;

        case 4: // Выбор кубической параболы
        {
            if (g_hCurrentGraphWindow != NULL) {
                DestroyWindow(g_hCurrentGraphWindow);
                g_hCurrentGraphWindow = NULL;
            }
            // Создаем и отображаем окно графика кубической параболы
            GraphDisplayWindow* graphDisplayWindow = new GraphDisplayWindow(g_hInstance, L"График степенной функции - кубическая парабола", PARABOLA2);
            graphDisplayWindow->Create();
            g_hCurrentGraphWindow = graphDisplayWindow->GetHandle();
        }
        break;
        }
        break;
    // Кейс обрабатывает сообщение о закрытии окна
    case WM_DESTROY:
        // Завершение приложения
        PostQuitMessage(0);
        return 0;
    /*Если обработчик не распознает сообщение или команду, оно передается 
    функции DefWindowProc, которая выполняет стандартную обработку сообщения*/
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf) // Рисование отрезка прямой линии
{
    if (hdc != NULL)
    {
        MoveToEx(hdc, Xs, Ys, 0); // Перемещение из точки (Xs, Ys)
        LineTo(hdc, Xf, Yf); // В точку (Xt, Yf)
    }
}

// Обработчик сообщений окна отображения графика
LRESULT CALLBACK GraphDisplayWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) // Было ли сообщение о создании окна
    {
        // Сохраняем указатель на объект окна
        LPCREATESTRUCT pCreate = (LPCREATESTRUCT)lParam; // Получаем доп. данные, которые содержат информацию о создании окна
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams); // Сохраняем указатель на объект окна
        return 0;
    }

    // Получаем указатель на объект окна
    GraphDisplayWindow* pGraphWindow = (GraphDisplayWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (pGraphWindow) // Существует ли указатель на объект окна 
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            // Не выходим из программы при закрытии окна графика
            return 0;
        // Кейс активируется, когда окно требует перерисовки
        case WM_PAINT:
        {
            /*Структура в Windows API, которая используется для хранения информации
            о процессе рисования окна в функции BeginPaint и EndPaint*/
            PAINTSTRUCT ps; 

            HDC hdc = BeginPaint(hwnd, &ps); // Функция начала рисования
            // hwnd - дескриптор окна, в котором будет происходить рисование
            // Получаем размеры клиентской области окна
            // ps - указатель на структуру PAINTSTRUCT, в которую будет записана информация о процессе рисования
           
            RECT rect;
            GetClientRect(hwnd, &rect); // Функция Windows API, которая получает размеры клиентской области указанного окна и записывает их в структуру RECT
            int width = rect.right - rect.left; //длина клиентской области
            int height = rect.bottom - rect.top; //высота

            // Координаты центра клиентской области
            int X = width / 2; 
            int Y = height / 2;

            //Полная отрисовка оси х
            line(hdc, 10, Y, width - 10, Y); //рисование горизонтальной оси
            line(hdc, width - 10, Y, width - 15, Y - 5); //верхнее гориз-ое оперение 
            line(hdc, width - 10, Y, width - 15, Y + 5); //нижнее гориз-ое оперение
           
            //Полная отрисовка оси у
            line(hdc, X, 10, X, height - 10); //рисование вертикальной оси
            line(hdc, X, 10, X + 5, 15); //правое верт-ое оперение 
            line(hdc, X, 10, X - 5, 15); //левое верт-ое оперение
          
            TCHAR buffer[32]; // Для хранения строковых данных
            TextOut(hdc, width - 10, Y + 10, L"X", 1); // Подпись оси x
            TextOut(hdc, X + 10, 5, L"Y", 1); // Подпись оси y
            TextOut(hdc, X + 5, Y + 5, L"0", 1); // 0 на пересечении осей

            // Определяем количество делений на оси X и Y
            int numDivisionsX = width / 50; // 50 - расстояние между делениями
            int numDivisionsY = height / 60;

            // Рисуем деления на оси X
            for (int i = 1; i < numDivisionsX; i++) {
                int xCoordPositive = X + i * (width / numDivisionsX); // Для положительных значений X
                int xCoordNegative = X - i * (width / numDivisionsX); // Для отрицательных значений X
                line(hdc, xCoordPositive, Y - 5, xCoordPositive, Y + 5);
                line(hdc, xCoordNegative, Y - 5, xCoordNegative, Y + 5);

                // Подписываем деление на оси X
                TCHAR buffer[32];
                swprintf_s(buffer, L"%d", i * 50);
                TextOut(hdc, xCoordPositive - 10, Y + 10, buffer, wcslen(buffer));
                swprintf_s(buffer, L"-%d", i * 50);
                TextOut(hdc, xCoordNegative - 15, Y + 10, buffer, wcslen(buffer));
            }

            // Рисуем деления на оси Y
            for (int i = 1; i < numDivisionsY; i++) {
                int yCoordPositive = Y - i * (height / numDivisionsY); // Для положительных значений Y
                int yCoordNegative = Y + i * (height / numDivisionsY); // Для отрицательных значений Y
                line(hdc, X - 5, yCoordPositive, X + 5, yCoordPositive);
                line(hdc, X - 5, yCoordNegative, X + 5, yCoordNegative);

                // Подписываем деление на оси Y
                TCHAR buffer[32];
                swprintf_s(buffer, L"%d", i * 100);
                TextOut(hdc, X + 10, yCoordPositive - 10, buffer, wcslen(buffer));
                swprintf_s(buffer, L"-%d", i * 100);
                TextOut(hdc, X + 10, yCoordNegative - 10, buffer, wcslen(buffer));
            }

            // Переменные для управления точностью и масштабом при отрисовке
            double step = 0.1; // Определяет шаг, с которым будут вычисляться точки графика функции
            double scale = 50.0; // Определяет масштаб, с которым значения функции будут преобразованы в пиксели на экране

            if (pGraphWindow->GetType() == LINEAR) // Проверка типа графика
            {
                // Установка цвета пера для отрисовки графика
                HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Красный цвет
                SelectObject(hdc, Pen);
                // Отрисовка графика линейной функции
                // Идем из крайней левой точки окна в правую
                MoveToEx(hdc, 0, Y - (int)(0.6 * (-X) + 3), NULL); // Начальная точка - крайнее левое положение окна по оси X и соответствующее значение Y
                for (int x = - X + 1; x < X; x++) {
                    int y = 0.6 * x + 3; // Вычисление значения линейной функции
                    LineTo(hdc, X + x, Y - y);
                }
                DeleteObject(Pen); // Освобождение ресурсов пера
            }
            
            else if (pGraphWindow->GetType() == SINUS) {
                HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // Зеленый цвет
                SelectObject(hdc, Pen);
                scale = 70.0;
                // Отрисовка графика синуса
                MoveToEx(hdc, 0, Y - (int)(sin(-X) * scale), NULL);
                for (int x = -X + 1; x < X; x++) {
                    int y = sin(x * step) * scale; // Значение функции синуса
                    LineTo(hdc, X + x, Y - y);
                }
                DeleteObject(Pen);
            }

            else if (pGraphWindow->GetType() == PARABOLA) {
                HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // Синий цвет
                SelectObject(hdc, Pen);
                // Отрисовка графика квадратичной функции
                MoveToEx(hdc, 0, Y - (int)(pow(-X / scale, 2) * scale), NULL);
                for (int x = -X + 1; x < X; x++) {
                    int y = pow(x / scale, 2) * scale; // Значение функции параболы
                    LineTo(hdc, X + x, Y - y);
                }
                DeleteObject(Pen);
            }
            else if (pGraphWindow->GetType() == PARABOLA2) {
                HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255)); // Розовый цвет
                SelectObject(hdc, Pen);
                // Отрисовка графика степенной функции
                MoveToEx(hdc, 0, Y - (int)(pow(-X / scale, 3) * scale), NULL);
                for (int x = -X + 1; x < X; x++) {
                    int y = pow(x / scale, 3) * scale; // Значение функции кубической параболы
                    LineTo(hdc, X + x, Y - y);
                }
                DeleteObject(Pen);
            }

            EndPaint(hwnd, &ps); // Завершение процесса рисования
        }
        return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

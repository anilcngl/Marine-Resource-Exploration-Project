#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <SDL2/SDL.h>

void drawGrid(SDL_Renderer *renderer, int numRows, int numCols, int cellSize) {
    // Grid çizmek için döngüler
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // Çizimleri saydam hale getir
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 50);

    for (int i = 0; i <= numRows; i++) {
        SDL_RenderDrawLine(renderer, 0, i * cellSize, numCols * cellSize, i * cellSize);
    }

    for (int i = 0; i <= numCols; i++) {
        SDL_RenderDrawLine(renderer, i * cellSize, 0, i * cellSize, numRows * cellSize);
    }
}

void drawCoordinates(SDL_Renderer *renderer, int *koordinat, int numPoints, int scaleFactor) {
    int ilkx = koordinat[0] , ilky = koordinat[1];
    for (int i = 0; i < numPoints - 2; i += 2) {
        int x1 = koordinat[i] * scaleFactor;
        int y1 = koordinat[i + 1] * scaleFactor;
        int x2 = koordinat[i + 2] * scaleFactor;
        int y2 = koordinat[i + 3] * scaleFactor;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
         if(koordinat[i+2] == ilkx && koordinat[i+3] == ilky){
            i +=2;
        }
        SDL_Color fillColor = {100, 0, 255, 0}; // Doldurma rengi (yarı saydam)
        // Her iki köşe arasındaki birim kareleri doldur
        fillLine(renderer, x1, y1, x2, y2, scaleFactor);
    }
}

void fillLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy;

    if (x1 < x2) {
        sx = 1;
    } else {
        sx = -1;
    }

    if (y1 < y2) {
        sy = 1;
    } else {
        sy = -1;
    }

    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        // Her birim kareyi boyamak için kullanılan işlev
        fillSquare(renderer, x1, y1, size);

        int e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x1 = x1 + sx;
        }
        if (e2 < dx) {
            err = err + dx;
            y1 = y1 + sy;
        }
    }

    fillSquare(renderer, x2, y2, size);
}

void fillSquare(SDL_Renderer *renderer, int x, int y, int size) {
    // Birim kareyi boyamak için kullanılan işlev
    SDL_Rect rect = {x, y, size, size};
    SDL_SetRenderDrawColor(renderer, 100, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

// Geri çağırma fonksiyonu
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((char*)userp)[0] = '\0';
    strncat((char*)userp, (char*)contents, size * nmemb);
    return size * nmemb;
}

// URL'den metin çeken işlev
char* UrlDenMetinCek(const char* url) {
    CURL* curl;
    CURLcode res;
    char* data = (char*)malloc(1024);

    // libcurl başlatılıyor
    curl = curl_easy_init();
    if (curl) {
        // URL'ye istek gönderiliyor
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Veriyi alırken kullanılacak callback işlevi ayarlanıyor
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        // İstek yapılıyor
        res = curl_easy_perform(curl);

        // Hata kontrolü
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        // libcurl temizleniyor
        curl_easy_cleanup(curl);
    }
    return data;
}

int SatirAl(int satir) {
    printf("Kaçıncı satırdaki koordinatları çizelim?: ");
    scanf("%d", &satir);
    return satir;
}

int SondajAl(int sondaj) {
    printf("Birim sondaj maliyeti ne kadar olacaktır?: ");
    scanf("%d", &sondaj);
    return sondaj;
}

int PlatformAl(int platform) {
    printf("Birim platform maliyeti ne kadar olacaktır?: ");
    scanf("%d", &platform);
    return platform;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char* alinanMetin;

    int Satir = SatirAl(0);
    printf("%d. satırdaki koordinatlar çizdirilecek.\n", Satir);

    int SondajBrm = SondajAl(0);
    int PlatformBrm = PlatformAl(0);

    // URL'yi belirt
    const char* url = "http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt";

    // URL'den metni çek
    alinanMetin = UrlDenMetinCek(url);

    // İlgili satırı al
    char* satirMetni = strtok(alinanMetin, "\n");
    for(int i = 1; i < Satir; i++) {
        satirMetni = strtok(NULL, "\n");
    }
    char alinanSatir[100];
    strcpy(alinanSatir, satirMetni);

    // Koordinatları al
    const char ayrac[] = "B(,)F";
    int koordinatlar[100]; // Koordinatları saklayacak dizi

    char *ayir = strtok(satirMetni, ayrac);
    int index = -1; // Koordinatları dizide saklamak için index

    while (ayir != NULL) {
        int koordinatDegeri = atoi(ayir); // Koordinatı tamsayıya dönüştürüyor
        koordinatlar[index++] = koordinatDegeri; // Diziye ekliyor
        ayir = strtok(NULL, ayrac);
    }
    free(alinanMetin);

    printf("Satır %d: %s\n", Satir, alinanSatir);

    // SDL başlatıyorum
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Ekran açıyorum
    SDL_Window *window = SDL_CreateWindow("Grafik Cizimi 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Renderer yapıyorum
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Olay döngüsü
    int quit = 0;
    SDL_Event e;
    int numRows = 600;
    int numCols = 800;
    int scaleFactor = 10;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // Izgarayı çiziyorum
        drawGrid(renderer, numRows, numCols, scaleFactor);
        SDL_SetRenderDrawColor(renderer, 100, 0, 255, 255);
        drawCoordinates(renderer, koordinatlar, index, scaleFactor);
        SDL_RenderPresent(renderer);
    }

    // SDL'i temizliyorum
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

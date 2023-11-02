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
    int ilkx = koordinat[0], ilky = koordinat[1];
    for (int i = 0; i < numPoints - 2; i += 2) {
        int x1 = koordinat[i] * scaleFactor;
        int y1 = koordinat[i + 1] * scaleFactor;
        int x2 = koordinat[i + 2] * scaleFactor;
        int y2 = koordinat[i + 3] * scaleFactor;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void fillPolygon(SDL_Renderer *renderer, int *coordinates, int numPoints, int scaleFactor) {
    int minX = coordinates[0] * scaleFactor;
    int minY = coordinates[1] * scaleFactor;
    int maxX = coordinates[0] * scaleFactor;
    int maxY = coordinates[1] * scaleFactor;

    // Koordinatları geçerken çokgenin sınırlarını belirle
    for (int i = 0; i < numPoints; i += 2) {
        int x = coordinates[i] * scaleFactor;
        int y = coordinates[i + 1] * scaleFactor;
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    // Belirli bir renk kullanarak çokgenin içini doldurun
    SDL_SetRenderDrawColor(renderer, 100, 0, 255, 50);

    // Çokgenin içini doldurmak için dikdörtgenleri çizin
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            if (pointInPolygon(x, y, coordinates, numPoints, scaleFactor)) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

int pointInPolygon(int x, int y, int *coordinates, int numPoints, int scaleFactor) {
    int c = 0;
    for (int i = 0, j = numPoints - 1; i < numPoints; j = i++) {
        int xi = coordinates[i * 2] * scaleFactor;
        int yi = coordinates[i * 2 + 1] * scaleFactor;
        int xj = coordinates[j * 2] * scaleFactor;
        int yj = coordinates[j * 2 + 1] * scaleFactor;

        if (((yi > y) != (yj > y)) &&
            (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
            c = !c;
        }
    }
    return c;
}

// libcurl'un geri çağırma fonksiyonu
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){
    ((char*)userp)[0] = '\0';
    strncat((char*)userp, (char*)contents, size * nmemb);
    return size * nmemb;
}

// URL'den metin çeken fonksiyon
char* UrlDenMetinCek(const char* url){
    CURL* curl;
    CURLcode res;
    char* data = (char*)malloc(1024); // Alınan verinin saklanacağı bellek alanı

    // libcurl başlatılıyor
    curl = curl_easy_init();
    if(curl) {
        // URL'ye istek gönderiliyor
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Veriyi alırken kullanılacak callback fonksiyonu ayarlanıyor
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        // İstek yapılıyor
        res = curl_easy_perform(curl);

        // Hata kontrolü
        if(res != CURLE_OK) {
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

    int Satir = SatirAl(Satir);
    printf("%d. satırdaki koordinatlar çizdirilecek.\n", Satir);

    int SondajBrm = SondajAl(SondajBrm);
    int PlatformBrm = PlatformAl(PlatformBrm);

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
        int koordinatDegeri = atoi(ayir); // Koordinatı tamsayıya dönüştür
        koordinatlar[index++] = koordinatDegeri; // Diziye ekle
        ayir = strtok(NULL, ayrac);
    }

    // Bellek temizliği
    free(alinanMetin);

    // Sonuçları görüntüle
    printf("Satır %d: %s\n", Satir, alinanSatir);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Grafik Cizimi 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Event handling loop
    int quit = 0;
    SDL_Event e;
    int numRows = 600;  // Örnek olarak 600 satır
    int numCols = 800;  // Örnek olarak 800 sütun
    int scaleFactor = 10;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
         // İlk olarak ızgarayı çiz
        drawGrid(renderer, numRows, numCols, scaleFactor);
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        drawCoordinates(renderer, koordinatlar, index, scaleFactor);
        fillPolygon(renderer, koordinatlar, index, scaleFactor); // Çokgenin içini doldur
        SDL_RenderPresent(renderer);
    }
    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

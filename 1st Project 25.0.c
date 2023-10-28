#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <SDL2/SDL.h>

// Ekran boyutları
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Kapalı alanın köşe noktaları (x, y) koordinatları
int points[] = {100, 100, 200, 100, 200, 200, 100, 200};

// libcurl'un geri çaðýrma fonksiyonu
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((char*)userp)[0] = '\0';
    strncat((char*)userp, (char*)contents, size * nmemb);
    return size * nmemb;
}

// URL'den metin çeken fonksiyon
char* UrlDenMetinCek(const char* url)
{
    CURL* curl;
    CURLcode res;
    char* data = (char*)malloc(1024); // Alýnan verinin saklanacaðý bellek alaný

    // libcurl baþlatýlýyor
    curl = curl_easy_init();

    if(curl)
    {
        // URL'ye istek gönderiliyor
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Veriyi alýrken kullanýlacak callback fonksiyonu ayarlanýyor
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        // Ýstek yapýlýyor
        res = curl_easy_perform(curl);

        // Hata kontrolü
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // libcurl temizleniyor
        curl_easy_cleanup(curl);
    }

    return data;
}

int SatirAl(int satir)
{

    printf("Kacinci satirdaki koordinatlari cizelim?:");
    scanf("%d",&satir);
    return satir;

}

int SondajAl(int sondaj)
{
    printf("Birim sondaj maliyeti ne kadar olacaktir?:");
    scanf("%d",&sondaj);
    return sondaj;

}

int PlatformAl(int platform)
{
    printf("Birim platform maliyeti ne kadar olacaktir?:");
    scanf("%d",&platform);
    return platform;

}

int main(int argc, char* argv[]) {

    char* alinanMetin;

    int Satir = SatirAl(Satir);
    printf("%d. satirdaki koordinatlar cizdirilecek.\n",Satir);

    int SondajBrm = SondajAl(SondajBrm);
    int PlatformBrm = PlatformAl(PlatformBrm);

    // URL'yi belirt
    const char* url = "http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt";

    // URL'den metni çek
    alinanMetin = UrlDenMetinCek(url);

    // Ýlgili satýrý al
    char* satirMetni = strtok(alinanMetin, "\n");
    for(int i = 1; i < Satir; i++)
    {
        satirMetni = strtok(NULL, "\n");
    }
    char alinanSatir[100];
    strcpy(alinanSatir,satirMetni);

    //Koordinatları al
    const char ayrac[] = "B(,)F";
    int koordinatlar[100]; // Koordinatları saklayacak dizi

    char *ayir = strtok(satirMetni, ayrac);
    int index = -1; // Koordinatları dizide saklamak için index

    while (ayir != NULL) {
        int koordinatDegeri = atoi(ayir); // Koordinatı tamsayıya dönüştür
        koordinatlar[index++] = koordinatDegeri; // Diziye ekle
        ayir = strtok(NULL, ayrac);
    }

    // Koordinatları ekrana yazdır
    for (int i = 0; i < index; i++) {
        printf("%d ", koordinatlar[i]);
    }

    // Bellek temizliði
    free(alinanMetin);

    // Sonuçlarý görüntüle
    printf("Satir %d: %s\n", Satir, alinanSatir);




    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

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

    // Set the color to render (in this case, it's blue)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 45);

    // Clear the screen
    SDL_RenderClear(renderer);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Event handling loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

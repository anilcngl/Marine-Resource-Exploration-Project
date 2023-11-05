#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <SDL2/SDL.h>

float CokgenAlaniBoya(SDL_Renderer *renderer, int *koordinat, int ilkKoordinat, int sonKoordinat, int sekil, int boyut)
{
    // Koordinatları saklamak için bir dizi oluştur
    int x[sonKoordinat - ilkKoordinat];
    int y[sonKoordinat - ilkKoordinat];

    // x ve y değerlerini ayır
    for (int i = 0; i < (sonKoordinat - ilkKoordinat)/2; i++)
    {
        x[i] = koordinat[ilkKoordinat + i * 2] * boyut;
        y[i] = koordinat[ilkKoordinat + i * 2 + 1] * boyut;
    }

    // Dizideki en küçük x ve y değerlerini bul
    int minX = x[0], minY = y[0];
    for (int i = 1; i < (sonKoordinat - ilkKoordinat)/2; i++)
    {
        if (x[i] < minX) minX = x[i];
        if (y[i] < minY) minY = y[i];
    }

    // Dizideki en büyük x ve y değerlerini bul
    int maxX = x[0], maxY = y[0];
    for (int i = 1; i < (sonKoordinat - ilkKoordinat)/2; i++)
    {
        if (x[i] > maxX) maxX = x[i];
        if (y[i] > maxY) maxY = y[i];
    }

    // Kapladığı alanın boyutunu bul
    int genislik = maxX - minX + 1;
    int yukseklik = maxY - minY + 1;

    //int parsel[genislik][yukseklik];
    //memset(parsel, 0, sizeof(parsel));

    // Kapladığı alanı boyat
    float alan=0;
    for (int i = 0; i < genislik; i++)
    {
        for (int j = 0; j < yukseklik; j++)
        {
            int testX = minX + i;
            int testY = minY + j;

            // Ray casting algoritması kullanarak test noktası poligonun içinde mi?
            int iceride = 0;
            for (int k = 0; k < (sonKoordinat - ilkKoordinat)/2; k++)
            {
                int nextK = (k + 1) % ((sonKoordinat - ilkKoordinat)/2);
                if (((y[k] > testY) != (y[nextK] > testY)) && (testX < (x[nextK] - x[k]) * (testY - y[k]) / (y[nextK] - y[k]) + x[k]))
                {
                    iceride = !iceride;
                }
            }

            if (iceride)
            {
                SDL_SetRenderDrawColor(renderer, 0, 220, 0, 250); // Yeşil renk, belirli saydamlık
                SDL_Rect rect = {testX, testY, boyut, boyut};
                SDL_RenderFillRect(renderer, &rect);
                //parsel[genislik-testX][yukseklik-testY]=1;
                alan+=0.01;
            }
        }
    }
    /*for(int i = 0; i < genislik; i+=10)
    {
        for(int j = 0; i < yukseklik; j+=10)
        {
            if(parsel[i][j]==1 && parsel[i+1][j]==1 && parsel[i][j+1]==1 && parsel[i+1][j+1]==1)
            {
                parsel[i][j]=2;
                parsel[i+1][j]=2;
                parsel[i][j+1]=2;
                parsel[i+1][j+1]=2;
            }
        }
    }*/
    printf("\nSekil-%d => Rezerv Alan: %.f ",sekil,alan);
    return alan;

}

void IzgaraCiz(SDL_Renderer *renderer, int satir, int sutun, int boyut)
{
    // Grid çizmek için döngüler
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100);

    for (int i = 0; i <= satir; i++)
    {
        SDL_RenderDrawLine(renderer, 0, i * boyut, sutun * boyut, i * boyut);
    }

    for (int i = 0; i <= sutun; i++)
    {
        SDL_RenderDrawLine(renderer, i * boyut, 0, i * boyut, satir * boyut);
    }
}

int KoordinatCiz(SDL_Renderer *renderer, int *koordinat, int index, int *siraYap, int boyut)
{
    int i = 0, j = 0;
    siraYap[j] = i;
    int ilkx = koordinat[0], ilky = koordinat[1];
    for ( ; i < index - 2; i += 2)
    {
        int x1 = koordinat[i] * boyut;
        int y1 = koordinat[i + 1] * boyut;
        int x2 = koordinat[i + 2] * boyut;
        int y2 = koordinat[i + 3] * boyut;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        if(koordinat[i+2] == ilkx && koordinat[i+3] == ilky)
        {
            j++,
            siraYap[j] = i + 4;
            ilkx = koordinat[i + 4];
            ilky = koordinat[i + 5];
            i +=2;
        }
    }
    return j;
}

// libcurl'un geri çağırma fonksiyonu
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((char*)userp)[0] = '\0';
    strncat((char*)userp, (char*)contents, size * nmemb);
    return size * nmemb;
}

// URL'den metin çeken fonksiyon
char* UrldenSatiriCek(const char* url)
{
    CURL* curl;
    CURLcode sonuc;
    char* data = (char*)malloc(1024); // 

    // libcurl başlatılıyor
    curl = curl_easy_init();

    if(curl)
    {
        // URL'ye istek gönderiliyor
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Veriyi alırken kullanılacak callback fonksiyonu ayarlanıyor
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        // İstek yapılıyor
        sonuc = curl_easy_perform(curl);

        // Hata kontrolü
        if(sonuc != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(sonuc));
        }

        // libcurl temizleniyor
        curl_easy_cleanup(curl);
    }

    return data;
}

int SatirAl(int satir)
{
bas:
    printf("Kacinci satirdaki koordinatlari cizelim?:");
    scanf("%d",&satir);
    if(satir <= 0)
    {
        printf("Lutfen gecerli bir sayi giriniz.\n");
        goto bas;
    }
    return satir;

}

int SondajAl(int sondaj)
{
bas:
    printf("Birim sondaj maliyeti ne kadar olacaktir?:");
    scanf("%d",&sondaj);
    if(10 < sondaj || sondaj< 1)
    {
        printf("Lutfen gecerli bir sondaj maliyeti birimi giriniz.\n");
        goto bas;
    }
    return sondaj;

}

int PlatformAl(int platform)
{
bas:
    printf("Birim platform maliyeti ne kadar olacaktir?:");
    scanf("%d",&platform);
    if(platform <= 0)
    {
        printf("Lutfen gecerli bir platform maliyeti birimi giriniz.\n");
        goto bas;
    }
    return platform;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    char* alinanMetin;

    int Satir = SatirAl(Satir);
    printf("%d. satirdaki koordinatlar cizdirilecek.\n",Satir);

    int SondajBrm = SondajAl(SondajBrm);
    int PlatformBrm = PlatformAl(PlatformBrm);

    // URL'yi belirt
    const char* url = "http://zplusorg.com/prolab.txt";

    // URL'den metni çek
    alinanMetin = UrldenSatiriCek(url);

    // İlgili satırı al
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

    while (ayir != NULL)
    {
        int koordinatDegeri = atoi(ayir);// Koordinatı tamsayıya dönüştür
        koordinatlar[index++] = koordinatDegeri; // Diziye ekle
        ayir = strtok(NULL, ayrac);
    }

    int siraYap[6];
    int siraYapIndex;

    // Bellek temizliği
    free(alinanMetin);


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Grafik Cizimi 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_SHOWN);

    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Event handling loop
    int quit = 0;
    SDL_Event e;
    int boyut = 10;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
        // Set the color to render (in this case, it's black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // Clear the screen
        SDL_RenderClear(renderer);

        // İlk olarak ızgarayı çiz
        IzgaraCiz(renderer, 720, 1080, boyut);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        siraYapIndex = KoordinatCiz(renderer, koordinatlar, index, siraYap, boyut);
        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /*for(int i=0; i<=siraYapIndex ; i++)
    {
        printf("-- %d ",siraYap[i]);
    }
    */

    // Yeni pencere oluştur
    SDL_Window *newWindow = SDL_CreateWindow("Yeni Pencere", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *newRenderer = SDL_CreateRenderer(newWindow, -1, SDL_RENDERER_ACCELERATED);


    quit = 0;
    int i = 0;
    float alan = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
        // Set the color to render (in this case, it's black)
        SDL_SetRenderDrawColor(newRenderer, 0, 0, 0, 255);
        // Do not clear the screen
        // SDL_RenderClear(newRenderer);

        // İlk olarak ızgarayı çiz

        IzgaraCiz(newRenderer, 720, 1080, boyut);

        if(i < siraYapIndex)
        {
            alan = alan + CokgenAlaniBoya(newRenderer, koordinatlar, siraYap[i], siraYap[i+1], i+1, boyut);
            i++;
            if(i == siraYapIndex)
                printf("\nToplam Rezerv Alan: %.f",alan);
        }

        // Update the screen
        SDL_RenderPresent(newRenderer);
    }
    SDL_DestroyRenderer(newRenderer);
    SDL_DestroyWindow(newWindow);

    SDL_Quit();

    return 0;
}

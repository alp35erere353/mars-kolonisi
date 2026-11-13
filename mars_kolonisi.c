#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Degeri 0-100 araligina sikistirir (IF ile) */
int sinir(int x) {
    if (x < 0)   return 0;
    if (x > 100) return 100;
    return x;
}

int main(void) {
    srand(time(NULL));

    /* ---- Oyun durumu ---- */
    int saglik, enerji, oksijen, su, yedekParca, modulHasari;
    int kilitMuhurlu = 0;
    int gun = 1, tur = 0;

    /* ---- Zorluga gore degisen degerler ---- */
    int firtinaTurSayisi, firtinaHasar;
    int kesifEsikYuksek, kesifEsikDusuk, korunmaTaban;

    char zorluk;
    int gecerli;

    printf("=== MARS KOLONISI YASAM DESTEK SIMULATORU ===\n");

    /* ---- Zorluk secimi ---- */
    do {
        gecerli = 1;
        printf("Zorluk seviyesi secin (K: Kolay, N: Normal, Z: Zor): ");
        scanf(" %c", &zorluk);

        switch (zorluk) {
            case 'k':
            case 'K':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 20; yedekParca = 4; modulHasari = 0;
                firtinaTurSayisi = 3; firtinaHasar = 5;
                kesifEsikYuksek = 60; kesifEsikDusuk = 35; korunmaTaban = 50;
                break;
            case 'n':
            case 'N':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 12; yedekParca = 3; modulHasari = 0;
                firtinaTurSayisi = 5; firtinaHasar = 8;
                kesifEsikYuksek = 50; kesifEsikDusuk = 25; korunmaTaban = 40;
                break;
            case 'z':
            case 'Z':
                saglik = 100; enerji = 100; oksijen = 80;
                su = 6; yedekParca = 2; modulHasari = 10;
                firtinaTurSayisi = 7; firtinaHasar = 12;
                kesifEsikYuksek = 40; kesifEsikDusuk = 15; korunmaTaban = 25;
                break;
            default:
                printf("Gecersiz seviye! Lutfen K, N veya Z girin.\n");
                gecerli = 0;
        }
    } while (!gecerli);

    printf("Zorluk secildi. Baslangic suyu: %d, yedek parca: %d\n", su, yedekParca);

    /* ---- ANA DONGU ---- */
    char komut;
    int oyunBitti = 0;      /* 0: devam, 1: cikis, 2: kaybetti, 3: kazandi */
    int turHarcandi;

    do {
        printf("\n[Gun %d | Tur %d] Komut (G,O,T,D,U,K,H,Q): ", gun, tur);
        scanf(" %c", &komut);
        turHarcandi = 0;

        switch (komut) {

            /* ---------- G: Kesif ---------- */
            case 'g':
            case 'G': {
                if (enerji < 15) {
                    printf("Yetersiz enerji! Kesif icin en az 15 enerji gerekli. (Komut iptal)\n");
                    break;
                }
                enerji -= 15;
                oksijen -= 10;
                printf("Kesif gorevine ciktin... (Enerji -15, Oksijen -10)\n");

                /* 1) Yaralanma kontrolu */
                int zar = rand() % 100;
                if (zar < 10 || modulHasari > 70) {
                    saglik -= 15;
                    printf("Zar: %d / Esik: 10 (veya hasar>70) -> YARALANDIN! Saglik -15\n", zar);
                } else {
                    printf("Zar: %d / Esik: 10 -> Yaralanma yok.\n", zar);
                }

                /* 2) Kesif sonucu: esik, mevcut duruma ve zorluga gore degisir */
                int esik;
                if (enerji > 40 && oksijen > 30) esik = kesifEsikYuksek;
                else                             esik = kesifEsikDusuk;

                int zar2 = rand() % 100;
                if (zar2 < esik) {
                    su += 4;
                    printf("Zar: %d / Esik: %d -> BASARILI: 4 birim su buldun!\n", zar2, esik);
                } else if (zar2 < esik + 20) {
                    yedekParca += 1;
                    printf("Zar: %d / Esik: %d -> Yedek parca buldun! (+1)\n", zar2, esik + 20);
                } else {
                    printf("Zar: %d / Esik: %d -> Eli bos dondun.\n", zar2, esik + 20);
                }
                turHarcandi = 1;
                break;
            }

            /* ---------- O: Oksijen uret ---------- */
            case 'o':
            case 'O': {
                if (enerji < 10) {
                    printf("Yetersiz enerji! Oksijen uretimi icin 10 enerji gerekli. (Komut iptal)\n");
                    break;
                }
                if (su < 2) {
                    printf("En az 2 birim su gerekli. (Komut iptal)\n");
                    break;
                }
                int miktar;
                printf("Kac birim su kullanilsin? (Mevcut: %d): ", su);
                if (scanf("%d", &miktar) != 1) {
                    while (getchar() != '\n');
                    printf("Gecersiz giris! (Komut iptal)\n");
                    break;
                }
                if (miktar < 2 || miktar > su) {
                    printf("Gecersiz miktar! 2 ile %d arasinda olmali. (Komut iptal)\n", su);
                    break;
                }

                int kullanilan, artik;
                if (miktar % 2 == 0) {
                    kullanilan = miktar;
                    artik = 0;
                } else {
                    kullanilan = miktar - 1;
                    artik = 1;
                }
                int uretilen = (kullanilan / 2) * 3;    /* 2 su -> 3 oksijen */
                su -= kullanilan;
                oksijen += uretilen;
                enerji -= 10;
                printf("%d su -> %d oksijen uretildi. Artik su: %d (Enerji -10)\n",
                       kullanilan, uretilen, artik);
                turHarcandi = 1;
                break;
            }

            /* ---------- T: Tamir ---------- */
            case 't':
            case 'T': {
                if (yedekParca == 0) {
                    printf("Yedek parca yok, tamir yapilamaz! (Komut iptal)\n");
                    break;
                }
                if (enerji < 10) {
                    printf("Yetersiz enerji! Tamir icin 10 enerji gerekli. (Komut iptal)\n");
                    break;
                }
                if (modulHasari == 0) {
                    printf("Modul hasari yok, tamire gerek yok. (Komut iptal)\n");
                    break;
                }

                int gereken, onarim;
                if (modulHasari <= 30) {
                    gereken = 1; onarim = 15;
                    printf("Hafif hasar. ");
                } else if (modulHasari <= 70) {
                    gereken = 2; onarim = 25;
                    printf("Orta hasar. ");
                } else {
                    gereken = 3; onarim = 35;
                    printf("Agir hasar. ");
                }

                int kullanilanParca = gereken;
                if (yedekParca < gereken) {
                    kullanilanParca = yedekParca;
                    onarim = onarim * kullanilanParca / gereken;
                }
                yedekParca -= kullanilanParca;
                modulHasari -= onarim;
                enerji -= 10;
                printf("%d parca harcandi, hasar -%d (Enerji -10)\n", kullanilanParca, onarim);
                turHarcandi = 1;
                break;
            }

            /* ---------- D: Durum raporu ---------- */
            case 'd':
            case 'D':
                printf("\n--- DURUM RAPORU ---\n");
                printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
                printf("Su: %d | Yedek parca: %d | Modul hasari: %d\n", su, yedekParca, modulHasari);
                printf("Hava kilidi: %s | Gun: %d | Tur: %d\n",
                       kilitMuhurlu == 1 ? "MUHURLU" : "ACIK", gun, tur);

                if (oksijen < 25)     printf("UYARI: Oksijen kritik seviyede!\n");
                if (saglik < 30)      printf("UYARI: Saglik kritik seviyede!\n");
                if (modulHasari > 70) printf("UYARI: Modul hasari kritik seviyede!\n");
                if (enerji < 20)      printf("UYARI: Enerji kritik seviyede!\n");

                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70 && enerji >= 20)
                    printf("TUM SISTEMLER NORMAL\n");
                break;                      /* D tur harcamaz */

            /* ---------- U: Uyu / dinlen ---------- */
            case 'u':
            case 'U':
                enerji += 20;
                if (oksijen < 20) {
                    saglik -= 5;
                    printf("Oksijen cok dusuk! Dinlenirken sagligin azaldi (-5).\n");
                } else {
                    if (enerji < 50) saglik += 15;
                    else             saglik += 10;
                    printf("Dinlendin. Enerji ve saglik artti.\n");
                }
                turHarcandi = 1;
                break;

            /* ---------- K: Kum firtinasi ---------- */
            case 'k':
            case 'K': {
                if (enerji < 10) {
                    printf("Yetersiz enerji! Firtina icin 10 enerji gerekli. (Komut iptal)\n");
                    break;
                }
                enerji -= 10;
                printf("\n=== KUM FIRTINASI BASLADI (%d tur) ===\n", firtinaTurSayisi);

                int hasarliTur = 0, toplamSaglikKaybi = 0, toplamOksijenKaybi = 0;

                for (int i = 1; i <= firtinaTurSayisi; i++) {
                    int carpan = (i % 2 == 0) ? 2 : 1;      /* cift turda ruzgar siddetlenir */
                    int hasar = firtinaHasar * carpan;

                    int korunmaEsigi;
                    if (modulHasari > 50) korunmaEsigi = korunmaTaban - 20;
                    else                  korunmaEsigi = korunmaTaban;

                    int zar = rand() % 100;
                    printf("Tur %d%s | Zar: %d / Korunma esigi: %d -> ",
                           i, carpan == 2 ? " (SIDDETLI RUZGAR)" : "", zar, korunmaEsigi);

                    if (zar < korunmaEsigi) {
                        printf("Firtinadan zarar gormeden korundun.\n");
                    } else if (zar < korunmaEsigi + 20) {
                        saglik -= hasar;
                        toplamSaglikKaybi += hasar;
                        hasarliTur++;
                        printf("Savruldun! Saglik -%d\n", hasar);
                    } else if (zar < korunmaEsigi + 40) {
                        if (kilitMuhurlu == 1) {
                            printf("Oksijen sizintisi, ama hava kilidi muhurlu. Atlandi.\n");
                            continue;
                        }
                        oksijen -= hasar;
                        toplamOksijenKaybi += hasar;
                        hasarliTur++;
                        printf("Oksijen sizintisi! Oksijen -%d\n", hasar);
                    } else {
                        modulHasari += hasar;
                        hasarliTur++;
                        printf("Modul hasar aldi! Hasar +%d\n", hasar);
                    }

                    saglik = sinir(saglik);
                    oksijen = sinir(oksijen);
                    modulHasari = sinir(modulHasari);

                    if (saglik == 0 || oksijen == 0) {
                        printf("Kritik seviye! Firtina aniden sonlandi.\n");
                        break;
                    }
                }

                printf("--- Firtina ozeti: %d turda hasar, toplam saglik kaybi %d, oksijen kaybi %d ---\n",
                       hasarliTur, toplamSaglikKaybi, toplamOksijenKaybi);
                turHarcandi = 1;
                break;
            }

            /* ---------- H: Hava kilidi sifresi ---------- */
            case 'h':
            case 'H': {
                if (kilitMuhurlu == 1) {
                    printf("Hava kilidi zaten muhurlu. (Komut iptal)\n");
                    break;
                }
                if (enerji < 15) {
                    printf("Yetersiz enerji! Sifre denemeleri icin 15 enerji gerekli. (Komut iptal)\n");
                    break;
                }

                char sifre = 'A' + (gun % 26);
                char tahmin = 0;
                int deneme = 0;

                printf("Ipucu: Sifre, alfabede A harfinden bugunun gun sayisi kadar ilerideki harftir.\n");

                do {
                    printf("Sifre (%d. deneme, en fazla 3): ", deneme + 1);
                    scanf(" %c", &tahmin);
                    if (tahmin >= 'a' && tahmin <= 'z')
                        tahmin = tahmin - 32;
                    deneme++;

                    if (tahmin != sifre) {
                        enerji -= 5;
                        if (tahmin < sifre) printf("Yanlis! Ileri git.\n");
                        else                printf("Yanlis! Geri git.\n");
                    }
                } while (tahmin != sifre && deneme < 3);

                if (tahmin == sifre) {
                    kilitMuhurlu = 1;
                    printf("DOGRU! Hava kilidi muhurlendi.\n");
                } else {
                    modulHasari += 20;
                    printf("3 deneme basarisiz! Kilit zorlandi, modul hasari +20.\n");
                }
                turHarcandi = 1;
                break;
            }

            /* ---------- Q: Cikis ---------- */
            case 'q':
            case 'Q':
                oyunBitti = 1;
                break;

            default:
                printf("Tanimsiz komut! (Tur harcanmadi)\n");
        }

        /* ---- Her komuttan sonra: sinirlandirma ---- */
        saglik = sinir(saglik);
        enerji = sinir(enerji);
        oksijen = sinir(oksijen);
        modulHasari = sinir(modulHasari);

        /* ---- Zaman akisi ---- */
        if (turHarcandi == 1) {
            tur++;
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                if (modulHasari > 70) saglik -= 5;
                printf(">>> Yeni gun basladi: Gun %d (oksijen -8)\n", gun);
                saglik = sinir(saglik);
                oksijen = sinir(oksijen);
            }
        }

        /* ---- Oyun sonu kontrolu ---- */
        if (oyunBitti == 0) {
            if (saglik == 0 || oksijen == 0) oyunBitti = 2;
            else if (gun > 15)               oyunBitti = 3;
        }

    } while (oyunBitti == 0);

    /* ---- SONUC RAPORU ---- */
    int hayattaGun = gun > 15 ? 15 : gun;
    if (oyunBitti == 3)      printf("\nKurtarma ekibi geldi, KAZANDIN!\n");
    else if (oyunBitti == 2) printf("\nHayatta kalamadin, KAYBETTIN.\n");
    else                     printf("\nOyundan cikildi.\n");

    int puan = (hayattaGun * 100) + saglik + oksijen - modulHasari;
    printf("Gun: %d | Toplam tur: %d\n", hayattaGun, tur);
    printf("Saglik: %d | Enerji: %d | Oksijen: %d | Hasar: %d\n", saglik, enerji, oksijen, modulHasari);
    printf("PUAN: %d\n", puan);

    return 0;
}

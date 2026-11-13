# Mars Kolonisi Yaşam Destek Simülatörü

C diliyle yazılmış, metin tabanlı bir hayatta kalma oyunudur. Oyuncu, Mars'taki küçük bir araştırma istasyonunda tek başına kalan bir mühendisi yönetir ve kurtarma ekibi gelene kadar **15 gün** hayatta kalmaya çalışır. Oyun tek karakterli komutlarla oynanır.

Proje, Algoritma ve Programlama dersi ödevi kapsamında geliştirilmiştir.

## Özellikler

- Üç zorluk seviyesi (Kolay, Normal, Zor); seviye başlangıç değerlerini ve olay eşiklerini değiştirir
- Büyük/küçük harf duyarsız komut sistemi (`switch-case` ve fall-through)
- Rastgele olaylarda her zar ve eşik değerinin ekrana yazdırılması (`srand(time(NULL))`, `rand() % 100`)
- `for` döngüsüyle kum fırtınası simülasyonu (`break` ve `continue` kullanımıyla)
- `do-while` döngüsüyle sınırlı denemeli hava kilidi şifresi
- Sağlık, enerji, oksijen ve modül hasarı için 0–100 sınırlandırması
- Oyun sonunda sonuç raporu ve puan

## Derleme ve çalıştırma

Bir C derleyicisi (ör. GCC) gereklidir.

```bash
gcc -Wall -o mars mars_kolonisi.c
./mars
```

Windows'ta çalıştırmak için `mars.exe` yazılır.

## Komutlar

| Komut | Açıklama | Koşul / maliyet |
|-------|----------|-----------------|
| `G` | Keşif görevi: su veya yedek parça bulunabilir, yaralanma riski vardır | En az 15 enerji; enerji −15, oksijen −10 |
| `O` | Oksijen üret: 2 birim su 3 birim oksijene dönüşür, tek sayıda suyun 1 birimi artık kalır | En az 10 enerji, en az 2 su; enerji −10 |
| `T` | Tamir: yedek parça harcanarak modül hasarı azaltılır (hafif, orta, ağır kademeler) | En az 10 enerji, yedek parça ve hasar gerekir |
| `D` | Durum raporu ve kritik uyarılar | Tur harcamaz |
| `U` | Uyu / dinlen: enerji ve sağlık artar; oksijen 20'nin altındaysa sağlık azalır | Enerji +20 |
| `K` | Kum fırtınası simülasyonu | En az 10 enerji; enerji −10 |
| `H` | Hava kilidi şifresi: en fazla 3 denemede doğru harfi bul | En az 15 enerji; yanlış denemede enerji −5 |
| `Q` | Çıkış | Tur harcamaz |

Tanımsız komutlar uyarı verir ve tur harcamaz. Yetersiz kaynakla verilen komut iptal edilir ve tur harcamaz.

## Zorluk seviyeleri

| Seviye | Su | Yedek parça | Başlangıç hasarı | Oksijen | Fırtına süresi | Fırtına hasarı |
|--------|----|-------------|------------------|---------|----------------|----------------|
| Kolay (`K`) | 20 | 4 | 0 | 100 | 3 tur | 5 |
| Normal (`N`) | 12 | 3 | 0 | 100 | 5 tur | 8 |
| Zor (`Z`) | 6 | 2 | 10 | 80 | 7 tur | 12 |

Zorluk seviyesi ayrıca keşifte su bulma eşiğini ve fırtınada korunma eşiğini de değiştirir.

## Oyun kuralları

- Geçerli her komut bir tur harcar. Her 3 turda bir yeni gün başlar; gün başında oksijen 8 azalır, modül hasarı 70'in üzerindeyse sağlık 5 azalır.
- Sağlık veya oksijen 0'a düşerse oyun kaybedilir.
- 15. gün tamamlanırsa kurtarma ekibi gelir ve oyun kazanılır.
- Hava kilidi mühürlüyse fırtınadaki oksijen sızıntıları engellenir.
- Oyun biterken (kazanma, kaybetme veya `Q`) hayatta kalınan gün, toplam tur, son durum değerleri ve puan gösterilir.

Puan formülü:

```
Puan = (gün × 100) + sağlık + oksijen − modül hasarı
```

## Dosya yapısı

```
.
├── mars_kolonisi.c   # oyunun tüm kaynak kodu
└── README.md
```

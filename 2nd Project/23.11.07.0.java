import java.util.*;
public class demiryolu {
    public static void main(String[] args) {
        Scanner tarayici = new Scanner(System.in);
        int[] Ankara = {0,150,100,250,200,0};
        int[] Bilecik = {150,0,50,150,50,200};
        int[] Eskişehir = {100,50,0,200,100,150};
        int[] Istanbul = {250,150,200,0,50,300};
        int[] Kocaeli = {200,50,100,50,0,250};
        int[] Konya = {0,200,150,300,250,0};

        System.out.print("Kullanmak istediginiz güzergahı seçiniz: ");
        int rota = tarayici.nextInt();
        switch(rota) {
                case 1: {
                System.out.println("Demiryolu Rotanız: Istanbul - Kocaeli - Bilecik - Eskişehir - Ankara - Eskişehir - Bilecik - Kocaeli - Istanbul.");
                System.out.print("Nereden bineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6): ");
                int binis = tarayici.nextInt();
                switch (binis) {
                    case 1: {
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                    case 2:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 3:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 4:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 5:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 6:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                    default:
                    System.out.println("Lütfen geçerli bir iniş noktası seçiniz...");
                }
            }
                break;
                case 2: {
                    System.out.println("Demiryolu Rotanız: Istanbul - Kocaeli - Bilecik - Eskişehir - Konya - Eskişehir - Bilecik - Kocaeli - Istanbul.");
                    System.out.print("Nereden bineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6): ");
                int binis = tarayici.nextInt();
                switch (binis) {
                    case 1: {
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                    case 2:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 3:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        if(binis==inis)
                        {System.out.println("Aynı yere gidiyosan neye biniyon puj!");}
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 4:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 5:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                     case 6:{
                        System.out.print("Nerede ineceksiniz? (Ankara: 1, Bilecik: 2, Eskişehir: 3, Istanbul: 4, Kocaeli: 5, Konya: 6):");
                        int inis = tarayici.nextInt();
                        System.out.println("Yol ücretiniz: "+Ankara [inis-1]+"");
                    }
                    break;
                       default:
                    System.out.println("Lütfen geçerli bir iniş noktası seçiniz...");
                }
            }
                break;
                default: {
                    System.out.println("Lütfen iki hattan birini seçiniz, demiryolu hat sayımız ikidir. Tekrar hat seçmeye yönlendiriliyorsunuz...");
                }
                
}
}
}

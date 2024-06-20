# Aplica-ie-de-Chat-multiclient
Aplicație de Chat multiclient


# 17.06.2024

- Am ales tema proiectului
- Am creat repository-ul pe git aferent temei
- Am inceput cercetarea asupra temei

# 18.06.2024

- Am definit principalele clase ale proiectului
- Am realizat conexiunea intre client si server la nivel de socketuri( conexiune ip si port)
- Am inceput tratarea posibilelor erori de conexiune
- Am definit clasa Utilizator care va fi responsabila de gestionarea userilor care vor participa la conversatii
- Am adaugat fuctionalitati de afisare ale utilizatorilor si i-am stocat intr-un vector pt ca serverul sa ii poata gestiona
- Am definit concepte de multithreading urmand ca mai apoi sa le dezvolt
- Am permis ca serverul sa poata primi mai multe conexiuni si sa le salveze in useri concreti

# 19.06.2024

- Am intampinat deficinte in a urca proiectul pe git si am refacut structura acestuia
- Am facut aplicatia sa devina multithread-ing, serverul ruland in paralel pt fiecare user dupa conectarea la server
- Am creat o baza de date care sa ajute la gestionarea userilor si mesajelor (momentan fara parola)
- Am creat o clasa care permite conexiunea cu baza de date
- Am inceput constructia meniului principal si a conversatiilor in interfata clientului
- Am efectuat diverse update-uri asupra codului pentru eficientizare

# 20.06.2024

- Am continuat dezvoltarea aplicatiei
- Am creat functii generice pentru a comunica cu Baza de date, necesare pentru afisarea "grafica" a meniului si a conversatiilor
- Am creat un meniu care permite afisarea tuturor userilor existenti in baza de date (in zilele urmatoare urmeaza creerea unor meniuri particularizate (conversatii doar cu contactele adaugate anterior ))
- Utilizatorul poate introduce un utilizator din meniul afisat si daca acesta exista, va putea incepe conversatia(pe viitor va trebui sa existe in lista de contacte);
- Am corectat diverse bug-uri anterioare ale codului si am adaugat noi verificari necesare in gasirea si gestionarea erorilor; 

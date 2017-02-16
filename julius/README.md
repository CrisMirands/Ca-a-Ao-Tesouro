# JULIUS

* Reconhecedor de voz (Open-Source): [julius-4.3.1](http://julius.osdn.jp/en_index.php?q=index-en.html#download_julius)
* Modelos acústicos: [LapsAPI](http://www.laps.ufpa.br/falabrasil/files/LaPSAM1.5-x64.tar.gz)
* Dependências necessárias:   
    sudo apt-get install libpulse-dev build-essential zlib1g-dev  
        flex libasound2-dev libesd0-dev libsndfile1-dev julius 
* Sistema utilizado:  
    Ubuntu 16.04, 64-bit  

## Instalação

* Instalação do Julius via source: 
(Só utilize esse tutorial se não instalou via terminal.)
    ./configure  
    make  
    sudo make install  

## Gerar arquivos

* Se modificar os arquivos .voca ou .grammar é preciso gerar novamente os arquivos “*.dfa”, “*.dict” e “*.term”. Vá para o diretório onde os respectivos arquivos estão e escreva no cmd:

    mkdfa [nome do arquivo .jconf sem o ".jconf"] 

## Teste

* Testar gramática:  
    generate [nome do arquivo jconf sem o ".jconf"]  

* Testar julius:  
    julius -input mic -C [nome].jconf  



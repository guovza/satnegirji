<h2>Davvisámegillii:</h2>

<p>Livččiigo dus beroštubmi dan prošektii? Jus dát geassá du miela, de don leat buresboahtin oassádallat! Eatnat giitu!</p>

<p>Mu vuosttas C++ / SQLite prošeakta, suoma-davvisáme-suoma sátnegirji. Sullii 50000 suoma-davvisáme sátnebárra dál. Lohken sámegiela 90-logus vierrogiellain ja lean vajálduhtán measta buot. Dál mun iskkán fas vehá hárjehallat go lean máhccan davás.</p>

<p>Mun álggahin čoakkildit sániid ovttas Kristian Guttorm sullii 30 jági dássá. Su jápmima maŋŋel lean  joatkán dán bárgu ain duollet dálle. Luobbal-Jovsset Esko ja Juvvá Lemet leaba veahkehan mu ollu. Ollu giitu! Easkabáliid ožžon Kotus <a href="https://kaino.kotus.fi/algu/index.php?t=etusivu&kkieli=sa">Álgu</a>-sániid John Anders Sikkus, ollu giitu! Ja dieđusge lean maiddái geavahan Pekka Sammallahti ja Jovnna-Ánde Vest sátnegirjjiid. Eatnat giitu!</p>

<p>Dán háve prográmma doaibmá Linuxis, macOSis ja Windowsis. Gávnnat prográmma <a href="https://github.com/guovza/satnegirji/releases">Releases-siiddus</a>. Hálidivččen dahkat maid web veršuvvna, muhto mus váilu sihke server ja ruhta.</p>

<p>Dárbbašan namalassii olbmuid geain lea buorre sámegiela dáidu muhto teknihkalaš máhttu lea maid buresboahtán.</p>

<h2>TODO</h2>


<p>All the work is done with 0€. <b>Please help and contribute</b>.</p>

<p>If you have new words or if you spot an error(s), please e-mail me skuolfi.relay980@passmail.net</p>

<p><b>Eŋgelasgiella-davvisápmi-eŋgelasgiella websátnegirji</b>, dán jagi áigge</p>

<h2>Mo mun geavahan dán prográmma / Instructions for end-users</h2>

<p>Precompiled binaries are available for macOS,64-bit Linux and Windows under the <a href="https://github.com/guovza/satnegirji/releases">Releases page</a>. Choose the package of your choice based on what OS your are running!</p>

<p>To use the program, just put the satnegirji binary and satnegirji.db satabase to the same directory ("satnegirji" recommended).</p>

<p><pre>unzip package.zip</pre></p>

<p>Then open a terminal and type the following in it:</p>

<p>
<pre>
cd satnegirji
./satnegirji
</pre>
</p>

<p>Wildcard mark <b>*</b> is supported with search options 1 and 2:</p>

<p>
<pre>
*loddi
boazo*
*lintu
maa*
*arvo*
</pre>
</p>



<h2>In English & some technical stuff</h2>

<p>A very simple sámi-finnish-sámi dictionary program written with C++. My first C++ program ever. The database I used is <a href="https://www.sqlite.org" target="_blank">SQLite</a>. It's very fast, flexible and it is already installed on your computer and smartphone.</p>

<p>Currently about 50000 word pairs, and there are some errors and some duplicates in SQLite database. The database needs some cleaning for sure.</p>

<p>Very small memory footprint, only about +-1MB. Very fast, only nano seconds and the results are ready.</p>

<p>To compile, put all files to same directory/folder ("satnegirji" recommended) and open a terminal and run:</p

<code>g++ -o satnegirji satnegirji.cpp utils.cpp -lsqlite3</code>

<p>To use the program, put the compiled binary excecutable "satnegirji" and the database file "satnegirji.db" to same directory ("satnegirji" recommended), open a terminal and run:</p>

<code>./satnegirji</code>

<p>If you have new words to be added, the easiest way is to create a csv file in a following format:</p>

<p><pre>"Pikkuvene","Gárbbis; unnislágán fanas"
"Köli, emäpuu","Gielas"
"Sauvoa","Goargŋut ~ goarpmut ~ čuoibmut; čuimmiin jođihit fatnasa vuosterávdnjái"
"Kumota (vene talveksi)","Golvet; gomihit fatnasa dálvái"
"Perälauta, selusta,liekko","Liehkku; fatnasa maŋŋegeahči, masa mohtor ásahuvvo"
"Keskiteljo","Luŋka; fielluin ráhkaduvvon fanasláhtti gasku fatnas"</pre></p>

<p>All other formats are of course also welcome. My e-mail is skuolfi.relay980@passmail.net</p>

<p>A csv file can then be inserted easily to the SQLite database with sqlite3 or sqlite3.exe</p>

<p>
<pre>
sqlite3 satnegirji.db
  
sqlite> PRAGMA encoding="UTF-8";
sqlite> create table skirjatmp (`suomi` TXT,`saame` TXT); 
sqlite> .mode csv
sqlite> .import /Users/tryhanen/Documents/satnegirji/lahkasanit.csv skirjatmp
sqlite> insert into skirja('suomi','saame') select DISTINCT suomi,saame from skirjatmp order by suomi;
sqlite> drop table skirjatmp;
</pre>
</p>

<p>To make sure the new words are also added FTS5 virtual table (full text search), do the following</p>

<p>
<pre>
sqlite> drop table suomisaame;
sqlite> create virtual table suomisaame USING FTS5(suomi,saame); 
sqlite> insert into suomisaame('suomi','saame') select distinct suomi,saame from skirja;
</pre>
</p>

<h2>Lisence - Liseansa</h2>

<p>Don sáhttát geavahit dán prográmma go čuovut Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International -liseansa.</p>

<p>This work is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International. To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/</p>





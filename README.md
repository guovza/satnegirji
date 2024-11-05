<h2>Davvisámegillii:</h2>

<p>Livččiigo dus beroštubmi dan prošektii? Jus dát geassá du miela, de don leat buresboahtin oassádallat! Eatnat giitu!</p>

<p>Mu vuosttas C++ / SQLite prošeakta, suoma-davvisáme-suoma sátnegirji. Sullii 44000 suoma-davvisámi sátnebárra dál ja várra 10000 duppalat mat fertejit čorget eret. Lohken sámegiela 90-logus vierrogiellain ja lean vajálduhtán measta buot. Dál mun iskkán fas vehá hárjehallat go lean máhccan davás.</p>

<p>Mun álggahin čoakkildit sániid ovttas Kristian Guttorm sullii 30 jági dássá. Su jápmima maŋŋel lean  joatkán dán bárgu ain duollet dálle. Luobbal-Jovsset Esko ja Juvvá Lemet leaba maiddái veahkehan mu. Ollu giitu! Ja dieđusge lean maiddái geavahan Pekka Sammallahti ja Jovnna-Ánde Vest sátnegirjjiid. Eatnat giitu!</p>

<p>Dán háve prográmma doaibmá Linuxis ja macOSis, áiggun vel dahkat Windows veršuvnna.</p>

<p>Dárbbašan namalassii olbmuid geain lea buorre sámegiela dáidu muhto teknihkalaš máhttu lea maid buresboahtán.</p>

<h2>Mo mun geavahan dán prográmma / Instructions for end-users</h2>

<p>Precompiled binaries are available for <a href="https://github.com/guovza/satnegirji/releases/tag/suoma-same-suoma-satnegirji-macos">macOS</a> and for 64-bit <a href="https://github.com/guovza/satnegirji/releases/tag/suoma-same-suoma-satnegirji-linux">Linux</a>. Windows version will be available a little bit later. Vuordde vehá!</p>

<p>To use the program, just put the satnegirji binary and satnegirji.db satabase to the same directory ("satnegirji" recommended).</p>

<p><pre>unzip package.zip</pre></p>

<p>Then open a terminal and type the following in it:</p>

<p>
<pre>
cd satnegirji
./satnegirji
</pre>
</p>

<p>Wildcard mark <b>*</b> is supported!!!!</p>

<h2>In English & some technical stuff</h2>

<p>A very simple sámi-finnish-sámi dictionary program written with C++. My first C++ program ever. The database I used is <a href="https://www.sqlite.org" target="_blank">SQLite</a>. It's very fast, flexible and it is already installed on your computer and smartphone.</p>

<p>Currently about 44000 word pairs, and there are some errors and a lot of duplicates in SQLite database. The database needs some cleaning for sure.</p>

<p>Very small memory footprint, only about +-1MB. Very fast, only nano seconds and the results are ready.</p>

<p>To compile, put all files to same directory/folder ("satnegirji" recommended) and open a terminal and run:</p

<code>g++ -o satnegirji satnegirji.cpp utils.cpp -lsqlite3</code>

<p>To use the program, put the compiled binary excecutable "satnegirji" and the database file "satnegirji.db" to same directory ("satnegirji" recommended), open a terminal and run:</p>

<code>./satnegirji</code>

<p>If you have new words to be added, the easiest way is to create a csv file in a following format:</p>

<p><pre>"suomen kielen sana, toinen synonyymi","sámegiela sátni, synonyma"
"sana","sátni"
"toinen sana","nubbi sátni"</pre></p>

<p>All other formats are of course also welcome.</p>

<p>A csv file can then be inserted easily to the SQLite database with sqlite3 or sqlite3.exe</p>

<p>
<pre>
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
sqlite> drop table suomisaame
sqlite> create virtual table suomisaame USING FTS5(suomi,saame); 
sqlite> insert into suomisaame('suomi','saame') select distinct suomi,saame from skirja;
</pre>
</p>

<h2>TODO</h2>

<p>Jus mus livččii eambbo áigi...</p>

<h2>Lisence - Liseansa</h2>

<p>Don sáhttát geavahit dan prográmma go čuovut Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International -liseansa.</p>

<p>This work is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International. To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/</p>





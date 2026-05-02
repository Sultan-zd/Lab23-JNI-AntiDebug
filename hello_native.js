Set-Content -Path hello_native.js -Value @"
console.log('[+] Script natif charge avec succes (Version Finale) !');

var adresseRecv = Module.findExportByName('libc.so', 'recv');

if (adresseRecv !== null) {
    console.log('[+] Super, recv trouvee a l adresse : ' + adresseRecv);
    Interceptor.attach(adresseRecv, {
        onEnter: function(args) {
            console.log('[+] Interception : fonction recv appelee !');
        }
    });
} else {
    console.log('[-] Erreur : recv introuvable.');
}
"@
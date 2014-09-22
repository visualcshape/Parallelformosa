/**
 * Created by Vodalok on 2014/9/23.
 */
var fs = require('fs');

var PATH = __dirname+'/../Classes';
var IGNORE = ['DS_Store','h'];

fs.readdir(PATH,function(err,files){
    var data = '';
    for(var i = 0 ; i < files.length ; i++){
        var split = files[i].split('.');
        split = split.map(function(delim){
            return delim;
        });
        if(IGNORE.indexOf(split[split.length-1])>-1){
            continue;
        }else{
            data+='../../Classes/'+files[i]+' \\'+'\n';
        }
    }
    fs.writeFile(__dirname+'/gen.txt',data,function(err){
        if(err){
            console.error(err);
        }else{
            console.log('DONE.');
        }
    });
});
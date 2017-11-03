// Business logic of index.html, handles getting json data

$(document).ready(function () {
    // da ripetere ogni intervallo con setInterval
    var interval = setInterval(function() {
        //console.log('Polling del JSON');
        $.getJSON("data.json", function(obj) {
            //console.log('polling...');
            $.each(obj, function(key, value){
                var unitOfMeasure = '';
                if(key == 'temps1' || key == 'tempext') unitOfMeasure = '°C';
                if(key == 'hums1') unitOfMeasure = '%';
                $('#' + key).html(value + ' ' + unitOfMeasure);
            });
        });
    }, 100);

});

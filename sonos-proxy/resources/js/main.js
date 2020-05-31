function setTokens(text) {
    let newElem = '<div id="#tokensBlock" class="card-body"><pre><code>' + 
                  text + 
                  '</code></pre></div>';
    $('#tokensBlock').replaceWith(newElem);
}

function loadTokens() {
    $.ajax({
        url: '/api/tokens',
        method: 'GET',
        success: function(result) {
            setTokens(JSON.stringify(result), null, 2);
            return false;
        },
        error: function(result) {
            setTokens('Unable to load tokens');
        }
    });
}

function init() {
    // Accordion toggle handlers
    $('#tokenAccordion').click(e => {
        $('#tokensBody').toggleClass('show');
    });
}

$(document).ready(function() {
    init();
    loadTokens();
})

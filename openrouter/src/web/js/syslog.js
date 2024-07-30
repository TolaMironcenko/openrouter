const syslogdata = document.querySelector('.syslogdata')
const refreshbutton = document.querySelector('#refreshbutton')
const openrouterfilter = document.querySelector('#openrouterfilter')
const authservicefilter = document.querySelector('#authservicefilter')
const realtimesyslogchekbox = document.querySelector('#realtimesyslog')

let openrouterfilterenable = false
let authservicefilerenable = false

const get_syslog_data = () => {
    fetch(routes.syslog(), {
        method: 'GET',
        // body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.text()).then(textdata => {
        if (openrouterfilterenable) {
            openrouterfilterfunc(textdata)
            return
        }
        if (authservicefilerenable) {
            authservicefilterfunc(textdata)
            return
        }
        syslogdata.innerHTML = textdata.replace(/\n/g, '<br>')
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

refreshbutton.addEventListener('click', () => {
    openrouterfilterenable = false
    authservicefilerenable = false
    get_syslog_data()
})

openrouterfilter.addEventListener('click', () => {
    openrouterfilterenable = true
    get_syslog_data()
})

authservicefilter.addEventListener('click', () => {
    authservicefilerenable = true
    openrouterfilterenable = false
    get_syslog_data()
})

const openrouterfilterfunc = (textdata) => {
    let syslogalldata = textdata.replace(/\n/g, '<br>')
    syslogalldata = syslogalldata.split("<br>")
    syslogalldata = syslogalldata.filter(data => {
        return data.includes("openrouter:")
    })
    syslogdata.innerHTML = ""
    for (const data in syslogalldata) {
        syslogdata.innerHTML += syslogalldata[data] + "<br>"
    }
    return
}

const authservicefilterfunc = (textdata) => {
    let syslogalldata = textdata.replace(/\n/g, '<br>')
    syslogalldata = syslogalldata.split("<br>")
    syslogalldata = syslogalldata.filter(data => {
        return data.includes("auth_service:")
    })
    syslogdata.innerHTML = ""
    for (const data in syslogalldata) {
        syslogdata.innerHTML += syslogalldata[data] + "<br>"
    }
    return
}

let syslogRealtimeIntervalId
realtimesyslogchekbox.addEventListener('click', () => {
    if (realtimesyslogchekbox.checked) {
        syslogRealtimeIntervalId = setInterval(get_syslog_data, 1000)
        // let socket = new WebSocket("ws://192.168.122.76/syslog");

        // socket.onmessage = function(event) {
        //     console.log(event.data)
        // };
    } else {
        clearInterval(syslogRealtimeIntervalId)
    }
})

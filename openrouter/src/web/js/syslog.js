const syslogdata = document.querySelector('.syslogdata')
const refreshbutton = document.querySelector('#refreshbutton')
const openrouterfilter = document.querySelector('#openrouterfilter')
const authservicefilter = document.querySelector('#authservicefilter')

let openrouterfilterenable = false
let authservicefilerenable = false

const get_syslog_data = () => {
    console.log("get_syslog_data")
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
        syslogdata.innerHTML = textdata.replace(/\n/g,'<br>')
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
    let syslogalldata = textdata.replace(/\n/g,'<br>')
    syslogalldata = syslogalldata.split("<br>")
    syslogalldata = syslogalldata.filter(data => {
        return data.includes("openrouter:")
    })
    console.log(syslogalldata)
    syslogdata.innerHTML = ""
    for (const data in syslogalldata) {
        syslogdata.innerHTML += syslogalldata[data] + "<br>"
    }
    return
}

const authservicefilterfunc = (textdata) => {
    let syslogalldata = textdata.replace(/\n/g,'<br>')
    syslogalldata = syslogalldata.split("<br>")
    syslogalldata = syslogalldata.filter(data => {
        return data.includes("auth_service:")
    })
    console.log(syslogalldata)
    syslogdata.innerHTML = ""
    for (const data in syslogalldata) {
        syslogdata.innerHTML += syslogalldata[data] + "<br>"
    }
    return
}
const syslogdata = document.querySelector('.syslogdata')
const refreshbutton = document.querySelector('#refreshbutton')
const openrouterfilter = document.querySelector('#openrouterfilter')

let openrouterfilterenable = false

const get_syslog_data = () => {
    console.log("get_syslog_data")
    fetch(routes.syslog(), {
        method: 'GET',
        // body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.text()).then(textdata => {
        if (openrouterfilterenable) {
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
        syslogdata.innerHTML = textdata.replace(/\n/g,'<br>')
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

refreshbutton.addEventListener('click', () => {
    openrouterfilterenable = false
    get_syslog_data()
})

openrouterfilter.addEventListener('click', () => {
    openrouterfilterenable = true
    get_syslog_data()
})

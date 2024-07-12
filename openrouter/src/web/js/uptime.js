const uptime = document.querySelector('#uptime')

const get_uptime = () => {
    fetch(routes.uptime()).then(data => data.json()).then(jsondata => {
        if (jsondata.uptime.hour === '0' && jsondata.uptime.minute === '0') {
            uptime.innerHTML = jsondata.uptime.second + "s "
            return
        }
        if (jsondata.uptime.hour === '0') {
            uptime.innerHTML = jsondata.uptime.minute + "m " + jsondata.uptime.second + "s "
            return
        }
        uptime.innerHTML = jsondata.uptime.hour + "h " + jsondata.uptime.minute + "m " + jsondata.uptime.second + "s "
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

setInterval(get_uptime, 1000)

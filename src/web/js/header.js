const settingssection = document.querySelector('.settingssection')
const settingsbutton = document.querySelector('#settingsbutton')
const syslogsection = document.querySelector('.syslogsection')
const syslogbutton = document.querySelector('#syslogbutton')
const infobutton = document.querySelector('#infobutton')
const infosection = document.querySelector('.infosection')

const headermenubutton = document.querySelector('#headermenubutton')
const headerlinks = document.querySelector('.links')
// const header = document.querySelector('.header')

let uptimeintervalid

settingsbutton.addEventListener('click', () => {
    settingssection.classList.add('active')
    syslogsection.classList.remove('active')
    infosection.classList.remove('active')
    clearInterval(uptimeintervalid)
    get_all_settings()
})

syslogbutton.addEventListener('click', () => {
    syslogsection.classList.add('active')
    settingssection.classList.remove('active')
    infosection.classList.remove('active')
    clearInterval(uptimeintervalid)
    get_syslog_data()
})

headermenubutton.addEventListener('click', () => {
    if (headerlinks.classList.contains('active')) {
        headerlinks.classList.remove('active')
        return
    }
    headerlinks.classList.add('active')
})

window.addEventListener('click', (e) => {
    if (e.target.id !== "headermenubutton" && !e.target.classList.contains('links')) {
        headerlinks.classList.remove('active')
    }
})

infobutton.addEventListener('click', () => {
    syslogsection.classList.remove('active')
    settingssection.classList.remove('active')
    infosection.classList.add('active')
    uptimeintervalid = setInterval(get_uptime, 1000)
})

// if (window.screenX > 600) {
//     header.removeChild(headermenubutton)
// } else {
//     header.prepend(headermenubutton)
// }

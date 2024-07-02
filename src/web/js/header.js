const settingssection = document.querySelector('.settingssection')
const settingsbutton = document.querySelector('#settingsbutton')
const syslogsection = document.querySelector('.syslogsection')
const syslogbutton = document.querySelector('#syslogbutton')

settingsbutton.addEventListener('click', () => {
    settingssection.classList.add('active')
    syslogsection.classList.remove('active')
})

syslogbutton.addEventListener('click', () => {
    syslogsection.classList.add('active')
    settingssection.classList.remove('active')
    get_syslog_data()
})

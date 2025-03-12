// admin.js
const puppeteer = require('puppeteer')

const visitUrl = async (url, cookieDomain, authToken) => {
    let browser =
        await puppeteer.launch({
            headless: true,
            pipe: true,
            dumpio: true,
            ignoreHTTPSErrors: true,
            args: [
                '--incognito',
                '--no-sandbox',
                '--disable-gpu',
                '--disable-software-rasterizer',
                '--disable-dev-shm-usage',
            ]
        })

    try {
        const page = await browser.newPage()

        try {
            // Set the authorization cookie
            await page.setCookie({
                name: 'auth_token',
                value: authToken,
                domain: cookieDomain,
                httpOnly: false,
                samesite: 'strict'
            })
            await page.goto(url, { timeout: 6000, waitUntil: 'networkidle2' })
        } finally {
            await page.close()
        }
    }
    finally {
        browser.close()
    }
}

module.exports = { visitUrl };

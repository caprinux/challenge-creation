const express = require('express');
const bodyParser = require('body-parser');
const bcrypt = require('bcrypt');
const { v4: uuidv4 } = require('uuid');
const sqlite3 = require('sqlite3').verbose();
const path = require('path');
const cookieParser = require('cookie-parser');
const jwt = require('jsonwebtoken');
const { visitUrl } = require('./admin');

const app = express();
const port = 3000;

const JWT_SECRET = process.env.JWT_SECRET;
if (!JWT_SECRET) {
  console.error('JWT_SECRET is not set. Please set it in the environment variables.');
  process.exit(1);
}

// Database setup
const db = new sqlite3.Database('./homework.db', (err) => {
  if (err) {
    console.error(err.message);
  }
  console.log('Connected to the homework database.');
});

db.run(`CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username TEXT UNIQUE,
  password TEXT,
  is_admin BOOLEAN DEFAULT 0
)`);

db.run(`CREATE TABLE IF NOT EXISTS submissions (
  id TEXT PRIMARY KEY,
  content TEXT,
  user_id INTEGER,
  FOREIGN KEY(user_id) REFERENCES users(id)
)`);

// Create admin user if it doesn't exist
const createAdminUser = async () => {
  const adminPassword = process.env.ADMIN_PASSWORD;
  if (!adminPassword) {
    console.error('ADMIN_PASSWORD is not set. Please set it in the environment variables.');
    process.exit(1);
  }
  const hashedPassword = await bcrypt.hash(adminPassword, 10);
  db.run(`INSERT OR IGNORE INTO users (username, password, is_admin) VALUES (?, ?, ?)`, 
    ['admin', hashedPassword, 1]);
};

createAdminUser();

// Middleware
app.use(bodyParser.urlencoded({ extended: true }));
app.use(cookieParser());
app.set('view engine', 'ejs');

// Authentication middleware
const authenticateUser = (req, res, next) => {
  const token = req.cookies.auth_token;
  if (token) {
    jwt.verify(token, JWT_SECRET, (err, decoded) => {
      if (err) {
        res.clearCookie('auth_token');
        return next();
      }
      req.user = decoded;
      next();
    });
  } else {
    next();
  }
};

app.use(authenticateUser);

// Routes
app.get('/', (req, res) => {
  if (req.user) {
    res.redirect('/submit_homework');
  } else {
    res.render('home');
  }
});

app.get('/register', (req, res) => {
  if (req.user) {
    res.redirect('/submit_homework');
  } else {
    res.render('register');
  }
});

app.post('/register', async (req, res) => {
  const { username, password } = req.body;
  const hashedPassword = await bcrypt.hash(password, 10);
  db.run('INSERT INTO users (username, password) VALUES (?, ?)', [username, hashedPassword], (err) => {
    if (err) {
      res.status(400).send('Username already exists');
    } else {
      res.redirect('/login');
    }
  });
});

app.get('/login', (req, res) => {
  if (req.user) {
    res.redirect('/submit_homework');
  } else {
    res.render('login');
  }
});

app.post('/login', async (req, res) => {
  const { username, password } = req.body;
  db.get('SELECT * FROM users WHERE username = ?', [username], async (err, user) => {
    if (user && await bcrypt.compare(password, user.password)) {
      const token = jwt.sign({ id: user.id, username: user.username, is_admin: user.is_admin }, JWT_SECRET, { expiresIn: '24h' });
      res.cookie('auth_token', token, { httpOnly: false, maxAge: 24 * 60 * 60 * 1000 }); // 24 hours
      res.redirect('/submit_homework');
    } else {
      res.redirect('/login');
    }
  });
});

app.get('/submit_homework', (req, res) => {
  if (req.user) {
    res.render('submit_homework', { 
      username: req.user.username,
      is_admin: req.user.is_admin
    });
  } else {
    res.redirect('/login');
  }
});

app.post('/submit_homework', (req, res) => {
  if (req.user) {
    const { content } = req.body;
    const uuid = uuidv4();
    db.run('INSERT INTO submissions (id, content, user_id) VALUES (?, ?, ?)', [uuid, content, req.user.id], async (err) => {
      if (err) {
        res.status(500).send('Error submitting homework');
      } else {
        // Generate admin token
        const adminToken = jwt.sign({ id: 1, username: 'admin', is_admin: true }, JWT_SECRET, { expiresIn: '1h' });
        // Admin visits the submission
        const submissionUrl = `http://localhost:${port}/submissions/${uuid}`;
        await visitUrl(submissionUrl, 'localhost', adminToken);
        res.redirect(`/submissions/${uuid}`);
      }
    });
  } else {
    res.redirect('/login');
  }
});

app.get('/submissions/:uuid', (req, res) => {
  const { uuid } = req.params;
  db.get('SELECT content FROM submissions WHERE id = ?', [uuid], (err, row) => {
    if (row) {
      res.render('submission', { content: row.content });
    } else {
      res.status(404).send('Submission not found');
    }
  });
});

app.get('/logout', (req, res) => {
  res.clearCookie('auth_token');
  res.redirect('/');
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});

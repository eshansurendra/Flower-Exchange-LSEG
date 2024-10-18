import React from 'react';
import { BrowserRouter as Router, Route, Routes } from 'react-router-dom';
import CSVUploader from './components/CSVUploader.jsx';
import CSVViewer from './components/CSVViewer.jsx';

const App = () => {
    return (
        <Router>
            <Routes>
                <Route path="/" element={<CSVUploader />} />
                <Route path="/view-csv" element={<CSVViewer />} />
            </Routes>
        </Router>
    );
};

export default App;
